WEBRTCTRUNK = F:/WORKSPACE/webrtc/src

INCLUDEPATH += .
INCLUDEPATH += $$WEBRTCTRUNK/
INCLUDEPATH += $$WEBRTCTRUNK/chromium/src/third_party/jsoncpp/source/include/
INCLUDEPATH += $$WEBRTCTRUNK/third_party/
INCLUDEPATH += $$WEBRTCTRUNK/third_party/webrtc/
INCLUDEPATH += $$WEBRTCTRUNK/webrtc/
INCLUDEPATH += $$WEBRTCTRUNK/net/third_party/nss/ssl
INCLUDEPATH += $$WEBRTCTRUNK/third_party/jsoncpp/overrides/include
INCLUDEPATH += $$WEBRTCTRUNK/third_party/jsoncpp/source/include

INCLUDEPATH += /usr/include/nss
INCLUDEPATH += /usr/include/nspr

PACKAGECONFIG[sdl] = "--enable-sdl,--disable-sdl,libsdl libxext,"
#CONFIG(debug,debug|release) {
#        WEBRTCBUILD = $$WEBRTCTRUNK/out/Debug
#} else {
        WEBRTCBUILD = $$WEBRTCTRUNK/out/Release
#}

#WEBRTCBUILD = $$WEBRTCTRUNK/out/Debug


LIBS += -Wl,-z,now
LIBS += -Wl,-z,relro
LIBS += -Wl,--fatal-warnings
LIBS += -Wl,-z,defs -pthread
LIBS += -Wl,--disable-new-dtags -m64
LIBS += -Wl,-O1
LIBS += -Wl,--as-needed

LIBS += -Wl,--start-group
LIBS += $$WEBRTCBUILD/obj/talk/libjingle_peerconnection.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libfield_trial_default.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/jsoncpp/libjsoncpp.a
LIBS += $$WEBRTCBUILD/obj/webrtc/base/librtc_base.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libwebrtc_common.a
LIBS += $$WEBRTCBUILD/obj/webrtc/base/librtc_base_approved.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/boringssl/libboringssl.a
LIBS += $$WEBRTCBUILD/obj/talk/libjingle_media.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_render_module.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libwebrtc_utility.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_coding_module.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libcng.a
LIBS += $$WEBRTCBUILD/obj/webrtc/common_audio/libcommon_audio.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libsystem_wrappers.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/openmax_dl/dl/libopenmax_dl.a
LIBS += $$WEBRTCBUILD/obj/webrtc/common_audio/libcommon_audio_sse2.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_encoder_interface.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libg711.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libpcm16b.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libwebrtc_opus.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/opus/libopus.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libg722.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libisac.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_decoder_interface.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libisac_common.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libilbc.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libred.a
LIBS += $$WEBRTCBUILD/obj/webrtc/librtc_event_log.a
LIBS += $$WEBRTCBUILD/obj/webrtc/librtc_event_log_proto.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/protobuf/libprotobuf_lite.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libneteq.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/librent_a_codec.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libmedia_file.a
LIBS += $$WEBRTCBUILD/obj/webrtc/common_video/libcommon_video.a
LIBS += $$WEBRTCBUILD/libyuv.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libjpeg_turbo/libjpeg_turbo.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libwebrtc.a
LIBS += $$WEBRTCBUILD/obj/webrtc/voice_engine/libvoice_engine.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_conference_mixer.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_processing.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudioproc_debug_proto.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_processing_sse2.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_device.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libbitrate_controller.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libpaced_sender.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/librtp_rtcp.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libremote_bitrate_estimator.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_capture_module.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_processing.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_processing_sse2.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libwebrtc_video_coding.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libwebrtc_h264.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libwebrtc_i420.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/video_coding/utility/libvideo_coding_utility.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/video_coding/codecs/vp8/libwebrtc_vp8.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_new.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_mmx.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_sse2.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_ssse3.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_sse4_1.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_avx.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx_new/libvpx_intrinsics_avx2.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/video_coding/codecs/vp9/libwebrtc_vp9.a
LIBS += $$WEBRTCBUILD/obj/webrtc/sound/librtc_sound.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libmetrics_default.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libjingle/xmllite/librtc_xmllite.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libjingle/xmpp/librtc_xmpp.a
LIBS += $$WEBRTCBUILD/obj/webrtc/p2p/librtc_p2p.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/usrsctp/libusrsctplib.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_capture_module_internal_impl.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_render_module_internal_impl.a
LIBS += $$WEBRTCBUILD/obj/talk/libjingle_p2p.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libsrtp/libsrtp.a
LIBS += -Wl,--end-group

LIBS += -L$$WEBRTCTRUNK/build/linux/debian_wheezy_amd64-sysroot/usr/lib/x86_64-linux-gnu/ \
-lX11 -lXfixes -lXi -lXcomposite -lXext -lXrender -ldl -lrt -lexpat -lXrandr -lm -ldl
