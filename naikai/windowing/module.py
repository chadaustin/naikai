process_idl([
    "nkIMenu.idl",
    "nkIWindow.idl",
    "nkIWindowingSystem.idl",
])

process_src([
    "win32/nkWindow.cpp",
    "win32/nkWindowingModule.cpp",
    "win32/nkWindowingSystem.cpp",
])
