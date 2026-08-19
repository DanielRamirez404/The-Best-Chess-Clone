cmake_minimum_required(VERSION 3.14)

project(TheBestChessClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Building project with CMake...")

if (UNIX)

	message(STATUS "Unix-like operating system detected")
	message(STATUS "Looking for SDL2 package")
	find_package(SDL2 REQUIRED)
	message(STATUS "SDL2 package found")
	message(STATUS "Looking for SDL2_image package")
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
	find_package(SDL2_image REQUIRED)
	message(STATUS "SDL2_image package found")
	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

elseif(WIN32)

	message(STATUS "Windows operating system detected")
	message(STATUS "Adding default Windows SDL2 directory for include files")
	include_directories("C:\\SDL2\\include")
	include_directories("C:\\SDL2_Image\\include")

	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
		message(STATUS "64-bit architecture detected")
		message(STATUS "Setting SDL2 and SDL2_Image library directories as the 64-bit default directories")
		set(SDL2_LIBRARIES "C:\\SDL2\\lib\\x64\\SDL2.lib;C:\\SDL2\\lib\\x64\\SDL2main.lib")
		set(SDL2_IMAGE_LIBRARIES "C:\\SDL2_Image\\lib\\x64\\SDL2_image.lib")
	else()
		message(STATUS "32-bit architecture detected")
		message(STATUS "Setting SDL2 and SDL2_Image library directories as the 32-bit default directories")
		set(SDL2_LIBRARIES "C:\\SDL2\\lib\\x86\\SDL2.lib;C:\\SDL2\\lib\\x86\\SDL2main.lib")
		set(SDL2_IMAGE_LIBRARIES "C:\\SDL2_Image\\lib\\x86\\SDL2_image.lib")
	endif()

endif()

message(STATUS "Creating executable from the project's source code")

set(SOURCES
	board.cpp
	boardMatrix.cpp
	chess.cpp
	coordinates.cpp
	main.cpp
	piece.cpp
)

add_executable(ChessClone ${SOURCES})

message(STATUS "Linking SDL2 and SDL2_Image libraries")
target_link_libraries(ChessClone ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# The game loads its assets with paths relative to the working directory
# (e.g. "res/board.bmp"), so the res/ folder is copied next to whatever
# executable CMake produces, whatever the build type or generator.
message(STATUS "Copying resource files next to the executable")
add_custom_command(TARGET ChessClone POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/res"
		"$<TARGET_FILE_DIR:ChessClone>/res"
	# A handful of assets are saved with an uppercase .PNG extension but are
	# requested in code with a lowercase .png one. That's harmless on
	# Windows' case-insensitive filesystem but breaks on Linux/macOS, so
	# lowercase copies are added alongside the originals as a safety net.
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/res/welcome.PNG"
		"$<TARGET_FILE_DIR:ChessClone>/res/welcome.png"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/res/you_win.PNG"
		"$<TARGET_FILE_DIR:ChessClone>/res/you_win.png"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/res/you_lose.PNG"
		"$<TARGET_FILE_DIR:ChessClone>/res/you_lose.png"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/res/draw.PNG"
		"$<TARGET_FILE_DIR:ChessClone>/res/draw.png"
)

if (WIN32)
	message(STATUS "Copying SDL2 DLLs next to the executable")
	add_custom_command(TARGET ChessClone POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/SDL2.dll"
			"$<TARGET_FILE_DIR:ChessClone>"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image.dll"
			"$<TARGET_FILE_DIR:ChessClone>"
	)
endif()
