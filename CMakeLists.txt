cmake_minimum_required(VERSION 2.8.12)
project(RunnerTmux)

find_package(ECM 5.12.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)

find_package (Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Quick)
find_package (KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets PlasmaQuick)

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
                      KF5::Plasma)
                      
install(TARGETS krunner_tmux DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-tmux.desktop DESTINATION ${SERVICES_INSTALL_DIR})
