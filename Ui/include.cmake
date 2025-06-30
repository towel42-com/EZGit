set(_PROJECT_NAME Ui)
set(FOLDER_NAME libs)

find_package(Qt6 REQUIRED COMPONENTS SerialPort Network Bluetooth Svg )

set(qtproject_UIS
    SelectGit.ui
    SelectGoal.ui
    SelectRepo.ui
    SelectBranch.ui
    SelectSandBox.ui
    Summary.ui
)

set(project_SRCS
    BasePage.cpp
    EZGit.cpp
    SelectGit.cpp
    SelectGoal.cpp
    SelectRepo.cpp
    SelectBranch.cpp
    SelectSandBox.cpp
    Summary.cpp
    UrlValidator.cpp
    UiStrings.cpp
)
 
set(qtproject_H
    BasePage.h
    EZGit.h
    SelectGit.h
    SelectGoal.h
    SelectRepo.h
    SelectBranch.h
    SelectSandBox.h
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
