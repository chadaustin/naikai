REQUIRES = [
    'base',
]

IDL_FILES = [
    'nkIMenu',
    'nkIWindow',
    'nkIWindowingService',
]

PUBLIC_HEADERS = [
    'nkIWin32Window.h',
]

COMPONENTS = [
  {
    'target': 'nkWindowing',
    'sources': [
	'nkMenu', 'nkWindow', 'nkWindowingModule', 'nkWindowingService'
    ],
    'srcdir': 'src/win32'
  }
]
