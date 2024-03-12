cmake_minimum_required(VERSION 3.16.0)
project(RunnerTmux)

set(KF6_MIN_VERSION 6.0.0)
find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)

find_package (Qt6 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Quick)
find_package (KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets KIO)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

kcoreaddons_add_plugin(krunner_tmux SOURCES tmux.cpp INSTALL_NAMESPACE "kf6/krunner")

target_link_libraries(krunner_tmux KF6::Runner 
                      Qt6::Widgets
                      KF6::I18n
                      KF6::Service
                      KF6::KIOCore
                      KF6::KIOFileWidgets
                      KF6::KIOWidgets)
