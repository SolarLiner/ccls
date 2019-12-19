project('ccls', 'cpp',
	version: '0.20',
	license: 'apache',
	meson_version: '>= 0.51',
	default_options: ['buildtype=debugoptimized', 'cpp_std=c++17']
)

llvm_prefix = get_option('LLVM_PREFIX')

if llvm_prefix == ''
	clang_exe = find_program('clang')
	llvm_prefix = run_command(clang_exe, '-print-resource-dir').stdout().strip()
else
	clang_exe = find_program(llvm_prefix / 'bin' / 'clang')
endif
message('Using Clang resources directory "@0@"'.format(llvm_prefix))
llvm_inc = llvm_prefix / 'include'

add_project_arguments(
	'-DCCLS_VERSION="@0@"'.format(meson.project_version()),
	'-DCLANG_RESOURCE_DIRECTORY="@0@"'.format(llvm_prefix),
	language: 'cpp'
)

rapidjson_dep = subproject('rapidjson').get_variable('rapidjson_dep')
threads_dep = dependency('threads', required: get_option('THREADS'))

deps = [rapidjson_dep]
if threads_dep.found()
	deps += [threads_dep]
endif

cp_exe = find_program('cp')
libgen = generator(cp_exe,
	arguments: ['@INPUT@', '@OUTPUT@'],
	output: '@PLAINNAME@'
)
if target_machine.system() == 'darwin'
	name = 'libclang-cpp.a'
	path = llvm_prefix / 'lib' / name
	tgt = custom_target(name,
		input: path,
		output: 'libclang-cpp.a',
		command: [cp_exe, '@INPUT@', '@OUTPUT@']
	)
	deps += [
		declare_dependency(
			link_with: tgt,
			include_directories: llvm_inc
		)
	]
else
	foreach lib : ['Index', 'Format', 'Tooling', 'ToolingInclusions', 'ToolingCore', 'Frontend', 'Parse', 'Serialization', 'Sema', 'AST', 'Lex', 'Driver', 'Basic']
	name = 'libclang'+lib+'.a'
	path = llvm_prefix / 'lib' / name
	tgt = custom_target(name,
		input: path,
		output: name,
		command: [cp_exe, '@INPUT@', '@OUTPUT@']
	)
	deps += declare_dependency(
		link_with: tgt,
		include_directories: llvm_inc
	)
	endforeach
endif

subdir('third_party')
deps += [siphash_dep]
subdir('src')
