REQUIRES = [
    'base',
]

COMPONENTS = [
{
    'target': 'nkWindowing',
    'sources': [
	'nkMenu', 'nkWindow', 'nkWindowingModule', 'nkWindowingService'
    ],
    'srcdir': 'win32'
}
]

IDL_FILES = [
    'nkIMenu',
    'nkIMenuItem',
    'nkIWindow',
    'nkIWindowingService',
]
