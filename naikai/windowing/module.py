REQUIRES = [
    'base',
]

COMPONENTS = [
{
    'target': 'nkWindowing',
    'sources': [ 'nkWindow', 'nkWindowingModule', 'nkWindowingService' ],
    'srcdir': 'win32'
}
]

IDL_FILES = [
    'nkIMenu',
    'nkIMenuItem',
    'nkIWindow',
    'nkIWindowingService',
]
