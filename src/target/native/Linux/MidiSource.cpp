#include "MidiSource.hpp"
#include <alsa/asoundlib.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <pthread.h>

class MidiSource::Impl
{
public:
   
   Impl()
   {
      // Set up the ALSA sequencer client.
      int result = snd_seq_open(&m_seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK );
      if ( result < 0 ) {
         std::cerr << "snd_se_open: error creating ALSA sequencer client object" << std::endl;
         return;
      }

      // Set client name.
      snd_seq_set_client_name(m_seq, MIDI_CLIENT_NAME);

      snd_seq_port_info_t *pinfo;
      snd_seq_port_info_alloca(&pinfo);
      snd_seq_port_info_set_capability(pinfo,
                                       SND_SEQ_PORT_CAP_WRITE |
                                       SND_SEQ_PORT_CAP_SUBS_WRITE);
      snd_seq_port_info_set_type(pinfo,
                                 SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                                 SND_SEQ_PORT_TYPE_APPLICATION);
      snd_seq_port_info_set_midi_channels(pinfo, 16);

      snd_seq_port_info_set_name(pinfo, MIDI_PORT_NAME);
      int vport = snd_seq_create_port(m_seq, pinfo);

      if (vport < 0) {
         std::cerr << "snd_seq_create_port: ALSA error creating virtual port" << std::endl;
         return;
      }
      vport = snd_seq_port_info_get_port(pinfo);

      // Start our MIDI input thread.
      pthread_attr_t attr;
      pthread_attr_init( &attr );
      pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );
      pthread_attr_setschedpolicy( &attr, SCHED_OTHER );
      
      int err = pthread_create( &m_thread, &attr, alsaMidiHandler, this);
      pthread_attr_destroy( &attr );
      if ( err ) {
         std::cerr << "Error starting MIDI input thread" << std::endl;
         return;
      }
      
      pthread_mutex_init(&m_mutex, NULL);
   }

   static void *alsaMidiHandler(void *ptr)
   {
      static_cast<MidiSource::Impl*>(ptr)->processMidi();
      return 0;
   }
   
   void processMidi()
   {
      int nBytes;
      int poll_fd_count;
      struct pollfd *poll_fds;

      snd_seq_event_t *ev;
      int result;

      result = snd_midi_event_new(0, &m_coder);
      if (result < 0) {
         std::cerr << "Error initializing MIDI event parser!" << std::endl;
         return;
      }

      unsigned char buffer[32];
      snd_midi_event_init(m_coder );
      snd_midi_event_no_status(m_coder, 1); // suppress running status messages

      poll_fd_count = snd_seq_poll_descriptors_count(m_seq, POLLIN) + 1;
      poll_fds = (struct pollfd*)alloca(poll_fd_count * sizeof(struct pollfd));
      snd_seq_poll_descriptors(m_seq, poll_fds + 1, poll_fd_count - 1, POLLIN);
      poll_fds[0].events = POLLIN;

      bool do_input = true;
      while (do_input)
      {
         if ( snd_seq_event_input_pending(m_seq, 1 ) == 0 )
         {
            // No data pending
            if (poll(poll_fds, poll_fd_count, -1) >= 0)
            {
               if (poll_fds[0].revents & POLLIN )
               {
                  bool dummy;
                  int res = ::read(poll_fds[0].fd, &dummy, sizeof(dummy));
                  (void) res;
               }
            }
            continue;
         }

         // If here, there should be data.
         result = snd_seq_event_input(m_seq, &ev );
         if ( result == -ENOSPC ) {
            std::cerr << "MIDI input buffer overrun!" << std::endl;
            continue;
         }
         else if ( result <= 0 ) {
            std::cerr << "Unknown MIDI input error:" << strerror(errno) << std::endl;
            continue;
         }

         bool doDecode = false;
         switch ( ev->type ) {

         case SND_SEQ_EVENT_PORT_SUBSCRIBED:
         case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
         case SND_SEQ_EVENT_QFRAME: // MIDI time code
         case SND_SEQ_EVENT_TICK: // 0xF9 ... MIDI timing tick
         case SND_SEQ_EVENT_CLOCK: // 0xF8 ... MIDI timing (clock) tick
         case SND_SEQ_EVENT_SENSING: // Active sensing
         case SND_SEQ_EVENT_SYSEX:
         default:
            doDecode = true;
         }

         if (doDecode) {
            nBytes = snd_midi_event_decode(m_coder, buffer, sizeof(buffer), ev);
            if (nBytes > 0) {
               pthread_mutex_lock(&m_mutex);
               MidiMessage msg;
               msg.length = std::min(int(sizeof(msg.data)), nBytes);
               std::copy(buffer, buffer + msg.length, msg.data);
               m_packets.push(msg);
               pthread_mutex_unlock(&m_mutex);
            }
         }

         snd_seq_free_event( ev );
      
      }

      snd_midi_event_free(m_coder);
      return;
   }
   
   const MidiMessage* read()
   {
      if (m_packets.empty()) return nullptr;
      
      pthread_mutex_lock(&m_mutex);
      m_last_returned_message = m_packets.front();
      m_packets.pop();
      pthread_mutex_unlock(&m_mutex);
      return &m_last_returned_message;
   }

   pthread_t m_thread;
   snd_seq_t *m_seq;
   snd_seq_port_subscribe_t *m_subscription;
   snd_midi_event_t *m_coder;
   std::queue<MidiMessage> m_packets;
   pthread_mutex_t m_mutex;
   MidiMessage     m_last_returned_message;
};

MidiSource::MidiSource():
   m_i(* new(Impl))
{
}

MidiSource::~MidiSource()
{
   delete &m_i;
}

const MidiMessage* MidiSource::read()
{
   return m_i.read();
}

MidiSource::MidiPorts* MidiSource::getMidiInPorts()
{
   return nullptr; // not implemented
}

MidiSource::MidiPorts* MidiSource::getMidiOutPorts()
{
   return nullptr; // not implemented
}

MidiSource::MidiSender*  MidiSource::getSender()
{
   return nullptr; // not implemented
}

MidiSource::MidiPorts*  MidiSource::getMidiControllers()
{
   return nullptr; // not implemented
}

void MidiSource::setCcFilter(uint8_t cc_num)
{
   // not implemented
   (void)cc_num;
}

void MidiSource::clearCcFilter(uint8_t cc_num)
{
   // not implemented
   (void)cc_num;
}
