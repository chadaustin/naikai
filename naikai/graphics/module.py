REQUIRES = [
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
