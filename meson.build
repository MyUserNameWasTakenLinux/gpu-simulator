project('gpu-simulator', 'cpp',
  version: '0.1',
  default_options: [
    'cpp_std=c++17',
    'cpp_link_args=-static-libstdc++',
    'c_link_args=-static-libstdc++'
  ])

vulkan_dep = dependency('vulkan')

executable('gpu-simulator',
  'main.cpp',
  'server.cpp',
  'util.cpp',
  'Execute.cpp',
  dependencies: [vulkan_dep],
  install: true)
