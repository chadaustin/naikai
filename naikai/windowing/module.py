REQUIRES = [
    'base',
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

IDL_FILES = [
    'nkIMenu',
    'nkIWindow',
    'nkIWindowingService',
]
