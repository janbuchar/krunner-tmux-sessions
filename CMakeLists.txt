cmake_minimum_required(VERSION 3.16.0)
project(RunnerTmux)

set(KF5_MIN_VERSION 5.72.0)
find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)

find_package (Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Quick)
find_package (KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets PlasmaQuick KIO)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(krunner_tmux_SRCS
    tmux.cpp
)

add_library(krunner_tmux MODULE ${krunner_tmux_SRCS})
target_link_libraries(krunner_tmux KF5::Runner Qt5::Widgets
                      KF5::I18n
                      KF5::Service
                      KF5::Plasma
		      KF5::KIOCore
		      KF5::KIOFileWidgets
		      KF5::KIOWidgets
		      KF5::KIONTLM)
                      
install(TARGETS krunner_tmux DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/krunner)
