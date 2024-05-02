#pragma once

#include "../../plugin.h"

//usage examples:
//- the host can tell a MIDI keyboard the plugin conforms to the drawbar organ profile, so the keyboard can set up its faders for that.
//- the host can send per-note articulations in MIDI 2.0 protocol note-on attributes if there's an active profile for this.

// background:
// There are 4 types of profiles: single channel, multi channel, group and function block (called "port" here).
// The profile specifications define the type for a specific profile.
//
// clap_plugin_midici_profiles.count()/get() return a list of profile_t structs.
// There'll typically be multiple entries for a single supported profile_id_t:
//   enabled single channel profiles: one entry per enabled channel. One entry if profile isn't enabled at all.
//   enabled multi channel profiles: one entry per enabled block of channels. One entry if profile isn't enabled at all.
//   enabled group profiles: one entry per enabled group. One entry if profile isn't enabled at all.
//   port profiles: one entry

//TODO : for completeness this extension could work for outputs too. A MIDI effect plugin might want to know 
//       the host can receive per-note articulations in note attributes for example.
//       can we simply define CLAP_EXT_MIDICI_PROFILES_PLUG_TO_HOST[] = "clap.midici-profiles.plug-to-host/draft/1" ???

static CLAP_CONSTEXPR const char CLAP_EXT_MIDICI_PROFILES[] = "clap.midici-profiles/draft/1";

#ifdef __cplusplus
extern "C" {
#endif

typedef profile_id_t profile_id[5];

typedef struct profile_t {
   profile_id_t profile_id;
   byte_t   enabled;        // 0 = off, 1 = on, 2 = always on
   byte_t   channel;        // single-channel profiles: channel is 0..255
                            // multi-channel profiles: channel is 0..255
                            // group profiles: channel is 16 * group
                            // port profiles: channel is 0
   uint16_t num_channels;   // single-channel profiles: num_channels is 1
                            // multi-channel profiles: num_channels is 1..256
                            // group profiles: num_channels is 0
                            // port profiles: num_channels is 0
};

typedef struct clap_plugin_midici_profiles {
   // Returns the number of profiles supported.
   // [main-thread]
   uint32_t(CLAP_ABI *count)(const clap_plugin_t   *plugin,
                             uint16_t               port_index);

   // Get a profile by index.
   // [main-thread]
   bool(CLAP_ABI *get)(const clap_plugin_t         *plugin,
                       uint16_t                     port_index,
                       uint32_t                     profile_index,
                       profile_t                   *profile);

   // Get number of bytes for profile specific data for the specified Inquiry Target.
   // If result=0 there's no data available for this Inquiry Target.
   // channel defines the destination:
   //   single-channel profiles: channel is 0..255
   //   multi-channel profiles: channel is 0..255
   //   group profiles: channel is 16 * group
   //   port profiles: channel is 0
   // [main-thread]
   uint32_t(CLAP_ABI *get_data_size)(const clap_plugin_t         *plugin,
                                     uint16_t                     port_index,
		                     uint32_t                     profile_index,
                                     byte_t                       channel,
                                     uint8_t                      inquiry_target);

   // Get profile specific data for the specified Inquiry Target.
   // buffer must be large enough to contain the number of bytes returned by get_data_size().
   // channel defines the destination:
   //   single-channel profiles: channel is 0..255
   //   multi-channel profiles: channel is 0..255
   //   group profiles: channel is 16 * group
   //   port profiles: channel is 0
   // [main-thread]
   bool(CLAP_ABI *get_data)(const clap_plugin_t         *plugin,
                            uint16_t                     port_index,
	                    uint32_t                     profile_index,
                            byte_t                       channel,
                            uint8_t                      inquiry_target,
                            const uint8_t               *buffer);

   // Enables a profile, so the plugin knows the host will use it.
   // Returns true if the profile is enabled when the function returns.
   // Can be called multiple times to enabled a profile for multiple channels or groups.
   // channel and num-channels define the destination:
   //   single-channel profiles: channel is 0..255, num_channels = 1
   //   multi-channel profiles: channel is 0..255, num_channels = 1..256
   //   group profiles: channel is 16 * group, num_channels = 0
   //   port profiles: channel is 0, num_channels = 0
   // Note for hosts: after calling this function count()/get() may have changed !!
   // Note for plugins: do not call clap_host_midici_profiles.changed.
   // [main-thread]
   bool(CLAP_ABI *enable)(const clap_plugin_t    *plugin,
                          uint16_t                port_index,
                          profile_id_t            profile,
                          byte_t                  channel,
                          uint16_t                num_channels);

   // Disables a profile.
   // Returns true if the profile is disabled when the function returns.
   // Note for hosts: after calling this function count()/get() may have changed !!
   // Note for plugins: do not call clap_host_midici_profiles.changed.
   // [main-thread]
   bool(CLAP_ABI *disable)(const clap_plugin_t  *plugin,
                           uint16_t              port_index,
                           profile_id_t          profile);
} clap_plugin_midici_profiles_t;

typedef struct clap_host_midici_profiles {
   // Informs the host that the available or active profiles changed.
   // [main-thread]
   void(CLAP_ABI *changed)(const clap_host_t *host);
} clap_host_midici_profiles_t;

#ifdef __cplusplus
}
#endif
