REQUIRES = [
    'base', # XXX should this be automatically inferred from 'windowing'?
    'windowing'
]

COMPONENTS = [
{
    'target': 'nkGraphics',
    'sources': [
        'nkRenderer',
	'nkRenderingModule',
    ],
    'srcdir': 'src/win32/opengl'
}
]

IDL_FILES = [
    'nkIRenderer'
]
