set(_PROJECT_NAME Ui)
set(FOLDER_NAME libs)

find_package(Qt6 REQUIRED COMPONENTS SerialPort Network Bluetooth Svg )

set(qtproject_UIS
    EnterComment.ui
    RunGit.ui
    SelectBranchOrTag.ui
    SelectCloneDir.ui
    SelectGit.ui
    SelectGoal.ui
    SelectRemote.ui
    SelectLocalDir.ui
    SetCredentials.ui
    Summary.ui
)

set(project_SRCS
    BasePage.cpp
    EnterComment.cpp
    EZGit.cpp
    RunGit.cpp
    SelectBranchOrTag.cpp
    SelectCloneDir.cpp
    SelectGit.cpp
    SelectGoal.cpp
    SelectRemote.cpp
    SelectLocalDir.cpp
    SetCredentials.cpp
    Summary.cpp
    UrlValidator.cpp
    UiStrings.cpp
)
 
set(qtproject_H
    BasePage.h
    EnterComment.h
    EZGit.h
    RunGit.h
    SelectBranchOrTag.h
    SelectCloneDir.h
    SelectGit.h
    SelectGoal.h
    SelectRemote.h
    SelectLocalDir.h
    SetCredentials.h
    Summary.h
    UrlValidator.h
)

set(project_H
    UiFwd.h
)

set(project_pub_LIB_DIRS 
)

set(project_pub_DEPS
    Qt6::Widgets
)

set(EXTRA_CMAKE_FILES
)

set(project_pri_LIB_DIRS 
)

set( project_pri_DEPS
)

set(qtproject_QRC
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)
