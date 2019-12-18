project('ccls', 'cpp', version: '0.20')

add_project_arguments('-DCCLS_VERSION=@0@'.format(meson.project_version()), language: 'cpp')

clang_exe = find_program('clang', get_option('LLVM_PREFIX') / 'bin' / 'clang')
llvm_inc = include_directories(get_option('LLVM_PREFIX') / 'include')

rapidjson_dep = subproject('rapidjson').get_variable('rapidjson_dep')
thread_dep = dependency('thread', required: get_option('THREADS'))

deps = [rapidjson_dep]
if thread_dep.found()
	deps += [thread_dep]
endif

if target_machine.system() == 'darwin'
	libclang_tgt = configure_file(
		input: get_option('LLVM_PREFIX') / 'lib' / 'libclang-cpp',
		output: 'libclang-cpp',
		copy: true
	)
	deps += [
		declare_dependency(
			link_with: libclang_tgt,
			include_directories: llvm_inc
		)
	]
else
	foreach lib : ['Index', 'Format', 'Tooling', 'ToolingInclusions', 'ToolingCore', 'Frontend', 'Parse', 'Serialization', 'Sema', 'AST', 'Lex', 'Driver', 'Basic']
	tgt = configure_file(
		input: get_option('LLVM_PREFIX') / 'lib' / 'libclang'+lib+'.a',
		output: lib,
		copy: true
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
