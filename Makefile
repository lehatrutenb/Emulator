CreateBuildLib:
	-mkdir Emulator_build
	cd Emulator_build && cmake ../Emulator

BuildCmakeRelease:
	cd Emulator_build && cmake ../Emulator -DSetDebugBuildType=OFF -DEnableCoverage=OFF
	cd Emulator_build && cmake --build .

BuildRunCmakeDebug:
	cd Emulator_build && cmake ../Emulator -DSetDebugBuildType=ON -DEnableCoverage=OFF
	cd Emulator_build && cmake --build .

	cd Emulator_build && ./Stack/StackTest && ./Emulator/EmulatorTest && ./Preprocessor/PreprocessorTest && ./Parser/ParserTest && ./Runner

BuildRunCmakeCoverage:
	cd Emulator_build && lcov --directory . --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1

	cd Emulator_build && cmake ../Emulator -DSetDebugBuildType=ON -DEnableCoverage=ON
	cd Emulator_build && cmake --build .

	cd Emulator_build && ./Stack/StackTest && ./Emulator/EmulatorTest && ./Preprocessor/PreprocessorTest && ./Parser/ParserTest

	cd Emulator_build && genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/
