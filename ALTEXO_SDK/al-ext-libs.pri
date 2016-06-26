WEBRTCTRUNK = /home/xors/workspace/ALTEXO/webrtc/src

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

# TODO: figure out if it is needed
#PACKAGECONFIG[sdl] = "--enable-sdl,--disable-sdl,libsdl libxext,"

#CONFIG(debug,debug|release) {
#        WEBRTCBUILD = $$WEBRTCTRUNK/out/Debug
#} else {
        WEBRTCBUILD = $$WEBRTCTRUNK/out/Release
#}


# TODO: seems to be unnesessary
#LIBS += -Wl,-z,now
#LIBS += -Wl,-z,relro
#LIBS += -Wl,--fatal-warnings
#LIBS += -Wl,-z,defs -pthread
#LIBS += -Wl,--disable-new-dtags -m64
#LIBS += -Wl,-O1
#LIBS += -Wl,--as-needed

LIBS += -Wl,--start-group

LIBS += $$WEBRTCBUILD/obj/webrtc/api/libjingle_peerconnection.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libfield_trial_default.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/jsoncpp/libjsoncpp.a
LIBS += $$WEBRTCBUILD/obj/webrtc/base/librtc_base.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libwebrtc_common.a
LIBS += $$WEBRTCBUILD/obj/webrtc/base/librtc_base_approved.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/boringssl/libboringssl.a
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

LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse2.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_ssse3.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_avx2.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_mmx.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_sse4_1.a

LIBS += $$WEBRTCBUILD/obj/webrtc/modules/video_coding/codecs/vp9/libwebrtc_vp9.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libmetrics_default.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libjingle/xmllite/librtc_xmllite.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libjingle/xmpp/librtc_xmpp.a
LIBS += $$WEBRTCBUILD/obj/webrtc/p2p/librtc_p2p.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/usrsctp/libusrsctplib.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_capture_module_internal_impl.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libsrtp/libsrtp.a

LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/boringssl/libboringssl_asm.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libcongestion_controller.a
LIBS += $$WEBRTCBUILD/obj/webrtc/pc/librtc_pc.a
LIBS += $$WEBRTCBUILD/obj/webrtc/media/librtc_media.a

# NEW LIBS
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libdesktop_capture.a
LIBS += $$WEBRTCBUILD/obj/webrtc/base/librtc_base_tests_utils.a
LIBS += $$WEBRTCBUILD/obj/webrtc/p2p/libstunprober.a
LIBS += $$WEBRTCBUILD/obj/webrtc/librtc_event_log_parser.a
LIBS += $$WEBRTCBUILD/obj/webrtc/tools/libagc_test_utils.a
LIBS += $$WEBRTCBUILD/obj/webrtc/tools/libvideo_quality_analysis.a
LIBS += $$WEBRTCBUILD/obj/webrtc/tools/libcommand_line_parser.a
LIBS += $$WEBRTCBUILD/obj/webrtc/tools/libframe_editing_lib.a
LIBS += $$WEBRTCBUILD/obj/webrtc/media/librtc_unittest_main.a
LIBS += $$WEBRTCBUILD/obj/webrtc/system_wrappers/libcpu_features_linux.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libtest_renderer.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libvideo_test_common.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libtest_common.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/librtp_test_utils.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libtest_main.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libhistogram.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libchannel_transport.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libfield_trial.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libmetrics.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libtest_support.a
LIBS += $$WEBRTCBUILD/obj/webrtc/test/libtest_support_main.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_codecs_test_framework.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudioproc_unittest_proto.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libacm_send_test.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libbwe_tools_util.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libneteq_unittest_tools.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libbwe_simulator.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudio_decoder_factory_interface.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libacm_receive_test.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libvideo_capture.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libneteq_test_support.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudioproc_test_utils.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libbuiltin_audio_decoder_factory.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/librtc_event_log_source.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libneteq_unittest_proto.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libpaced_sender.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libaudioproc_protobuf_utils.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libdesktop_capture_differ_sse2.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libneteq_test_tools.a
LIBS += $$WEBRTCBUILD/obj/webrtc/modules/libisac_fix.a
LIBS += $$WEBRTCBUILD/obj/webrtc/libvideo_quality_test.a
LIBS += $$WEBRTCBUILD/obj/third_party/gflags/libgflags.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/testing/libgtest.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/testing/libgmock.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/yasm/libgenperf_libs.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/protobuf/libprotobuf_full_do_not_use.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/protobuf/libprotoc_lib.a
LIBS += $$WEBRTCBUILD/obj/chromium/src/third_party/libvpx/libvpx_intrinsics_avx.a

LIBS += -Wl,--end-group

#LIBS += -L$$WEBRTCTRUNK/build/linux/debian_wheezy_amd64-sysroot/usr/lib/x86_64-linux-gnu/ \
LIBS += -lX11 -lXfixes -lXi -lXcomposite -lXext -lXrender -ldl -lrt -lexpat -lXrandr -lm -ldl
