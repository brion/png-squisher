png_squisher_sources = [
  'main.c',
  'png-squisher-window.c',
]

png_squisher_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

png_squisher_sources += gnome.compile_resources('png-squisher-resources',
  'png-squisher.gresource.xml',
  c_name: 'png_squisher'
)

executable('png-squisher', png_squisher_sources,
  dependencies: png_squisher_deps,
  install: true,
)
