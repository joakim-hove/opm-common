# This file sets up five lists:
#	MAIN_SOURCE_FILES     List of compilation units  which will be included in
#	                      the library. If it isn't on this list, it won't be
#	                      part of the library. Please try to keep it sorted to
#	                      maintain sanity.
#
#	TEST_SOURCE_FILES     List of programs that will be run as unit tests.
#
#	TEST_DATA_FILES       Files from the source three that should be made
#	                      available in the corresponding location in the build
#	                      tree in order to run tests there.
#
#	EXAMPLE_SOURCE_FILES  Other programs that will be compiled as part of the
#	                      build, but which is not part of the library nor is
#	                      run as tests.
#
#	PUBLIC_HEADER_FILES   List of public header files that should be
#	                      distributed together with the library. The source
#	                      files can of course include other files than these;
#	                      you should only add to this list if the *user* of
#	                      the library needs it.

list (APPEND MAIN_SOURCE_FILES
      src/opm/common/data/SimulationDataContainer.cpp
      src/opm/common/OpmLog/CounterLog.cpp
      src/opm/common/OpmLog/EclipsePRTLog.cpp
      src/opm/common/OpmLog/LogBackend.cpp
      src/opm/common/OpmLog/Logger.cpp
      src/opm/common/OpmLog/LogUtil.cpp
      src/opm/common/OpmLog/OpmLog.cpp
      src/opm/common/OpmLog/StreamLog.cpp
      src/opm/common/OpmLog/TimerLog.cpp
      src/opm/common/utility/ActiveGridCells.cpp
      src/opm/common/utility/numeric/MonotCubicInterpolator.cpp
      src/opm/common/utility/parameters/Parameter.cpp
      src/opm/common/utility/parameters/ParameterGroup.cpp
      src/opm/common/utility/parameters/ParameterTools.cpp
      src/opm/common/utility/numeric/calculateCellVol.cpp
      src/opm/common/utility/TimeService.cpp
)
if(ENABLE_ECL_INPUT)
  list(APPEND MAIN_SOURCE_FILES
    src/opm/json/JsonObject.cpp
    src/opm/parser/eclipse/Deck/Deck.cpp
    src/opm/parser/eclipse/Deck/DeckItem.cpp
    src/opm/parser/eclipse/Deck/DeckValue.cpp
    src/opm/parser/eclipse/Deck/DeckKeyword.cpp
    src/opm/parser/eclipse/Deck/DeckRecord.cpp
    src/opm/parser/eclipse/Deck/DeckOutput.cpp
    src/opm/parser/eclipse/Deck/DeckSection.cpp
    src/opm/parser/eclipse/Deck/UDAValue.cpp
    src/opm/parser/eclipse/Python/Python.cpp
    src/opm/parser/eclipse/EclipseState/AquiferCT.cpp
    src/opm/parser/eclipse/EclipseState/Aquifetp.cpp
    src/opm/parser/eclipse/EclipseState/Aquancon.cpp
    src/opm/parser/eclipse/EclipseState/checkDeck.cpp
    src/opm/parser/eclipse/EclipseState/Eclipse3DProperties.cpp
    src/opm/parser/eclipse/EclipseState/EclipseConfig.cpp
    src/opm/parser/eclipse/EclipseState/EclipseState.cpp
    src/opm/parser/eclipse/EclipseState/EndpointScaling.cpp
    src/opm/parser/eclipse/EclipseState/Edit/EDITNNC.cpp
    src/opm/parser/eclipse/EclipseState/Grid/FieldProps.cpp
    src/opm/parser/eclipse/EclipseState/Grid/FieldPropsManager.cpp
    src/opm/parser/eclipse/EclipseState/Grid/Box.cpp
    src/opm/parser/eclipse/EclipseState/Grid/BoxManager.cpp
    src/opm/parser/eclipse/EclipseState/Grid/EclipseGrid.cpp
    src/opm/parser/eclipse/EclipseState/Grid/FaceDir.cpp
    src/opm/parser/eclipse/EclipseState/Grid/FaultCollection.cpp
    src/opm/parser/eclipse/EclipseState/Grid/Fault.cpp
    src/opm/parser/eclipse/EclipseState/Grid/FaultFace.cpp
    src/opm/parser/eclipse/EclipseState/Grid/GridDims.cpp
    src/opm/parser/eclipse/EclipseState/Grid/GridProperties.cpp
    src/opm/parser/eclipse/EclipseState/Grid/GridProperty.cpp
    src/opm/parser/eclipse/EclipseState/Grid/MULTREGTScanner.cpp
    src/opm/parser/eclipse/EclipseState/Grid/NNC.cpp
    src/opm/parser/eclipse/EclipseState/Grid/Operate.cpp
    src/opm/parser/eclipse/EclipseState/Grid/PinchMode.cpp
    src/opm/parser/eclipse/EclipseState/Grid/SatfuncPropertyInitializers.cpp
    src/opm/parser/eclipse/EclipseState/Grid/setKeywordBox.cpp
    src/opm/parser/eclipse/EclipseState/Grid/TransMult.cpp
    src/opm/parser/eclipse/EclipseState/InitConfig/Equil.cpp
    src/opm/parser/eclipse/EclipseState/InitConfig/FoamConfig.cpp
    src/opm/parser/eclipse/EclipseState/InitConfig/InitConfig.cpp
    src/opm/parser/eclipse/EclipseState/IOConfig/IOConfig.cpp
    src/opm/parser/eclipse/EclipseState/IOConfig/RestartConfig.cpp
    src/opm/parser/eclipse/EclipseState/Runspec.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionAST.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionContext.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionResult.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/Actdims.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/Actions.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionX.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionParser.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ActionValue.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/ASTNode.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/Condition.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Action/PyAction.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/ArrayDimChecker.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Events.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/Group.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GuideRate.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GuideRateConfig.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GuideRateModel.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GConSale.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GConSump.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Group/GTNode.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/injection.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MessageLimits.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/Compsegs.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/Segment.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/WellSegments.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/updatingConnectionsWithSegments.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/SpiralICD.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/MSW/Valve.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/OilVaporizationProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/RFTConfig.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Schedule.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/SummaryState.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/TimeMap.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Tuning.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/Connection.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/eval_uda.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/Well.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellConnections.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WList.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WListManager.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellEconProductionLimits.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellFoamProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellInjectionProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellPolymerProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellBrineProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellTracerProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellProductionProperties.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellTestConfig.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/Well/WellTestState.cpp
    src/opm/parser/eclipse/EclipseState/SimulationConfig/SimulationConfig.cpp
    src/opm/parser/eclipse/EclipseState/SimulationConfig/ThresholdPressure.cpp
    src/opm/parser/eclipse/EclipseState/SummaryConfig/SummaryConfig.cpp
    src/opm/parser/eclipse/EclipseState/Tables/ColumnSchema.cpp
    src/opm/parser/eclipse/EclipseState/Tables/JFunc.cpp
    src/opm/parser/eclipse/EclipseState/Tables/PvtxTable.cpp
    src/opm/parser/eclipse/EclipseState/Tables/SimpleTable.cpp
    src/opm/parser/eclipse/EclipseState/Tables/PolyInjTables.cpp
    src/opm/parser/eclipse/EclipseState/Tables/TableColumn.cpp
    src/opm/parser/eclipse/EclipseState/Tables/TableContainer.cpp
    src/opm/parser/eclipse/EclipseState/Tables/TableIndex.cpp
    src/opm/parser/eclipse/EclipseState/Tables/TableManager.cpp
    src/opm/parser/eclipse/EclipseState/Tables/TableSchema.cpp
    src/opm/parser/eclipse/EclipseState/Tables/Tables.cpp
    src/opm/parser/eclipse/EclipseState/Tables/Rock2dTable.cpp
    src/opm/parser/eclipse/EclipseState/Tables/Rock2dtrTable.cpp
    src/opm/parser/eclipse/EclipseState/Tables/PvtwsaltTable.cpp
    src/opm/parser/eclipse/EclipseState/Tables/BrineDensityTable.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQASTNode.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQParams.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQParser.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQSet.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQActive.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQAssign.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQDefine.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQEnums.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQConfig.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQContext.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQFunction.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQFunctionTable.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQInput.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/VFPInjTable.cpp
    src/opm/parser/eclipse/EclipseState/Schedule/VFPProdTable.cpp
    src/opm/parser/eclipse/Parser/ErrorGuard.cpp
    src/opm/parser/eclipse/Parser/ParseContext.cpp
    src/opm/parser/eclipse/Parser/Parser.cpp
    src/opm/parser/eclipse/Parser/ParserEnums.cpp
    src/opm/parser/eclipse/Parser/ParserItem.cpp
    src/opm/parser/eclipse/Parser/ParserKeyword.cpp
    src/opm/parser/eclipse/Parser/ParserRecord.cpp
    src/opm/parser/eclipse/Parser/raw/RawKeyword.cpp
    src/opm/parser/eclipse/Parser/raw/RawRecord.cpp
    src/opm/parser/eclipse/Parser/raw/StarToken.cpp
    src/opm/parser/eclipse/Units/Dimension.cpp
    src/opm/parser/eclipse/Units/UnitSystem.cpp
    src/opm/parser/eclipse/Utility/Functional.cpp
    src/opm/parser/eclipse/Utility/Stringview.cpp
  )


# This list is only used to register a CMake dependency between the the python
# extension and the corresponding C++ wrapper files. The cpp files actually
# listed here are repeated in the actual definition of the extension in the
# setup.py file.
  list( APPEND PYTHON_CXX_SOURCE_FILES
          python/cxx/connection.cpp
          python/cxx/converters.cpp
          python/cxx/deck.cpp
          python/cxx/deck_keyword.cpp
          python/cxx/eclipse_io.cpp
          python/cxx/eclipse_3d_properties.cpp
          python/cxx/field_props.cpp
          python/cxx/eclipse_config.cpp
          python/cxx/eclipse_grid.cpp
          python/cxx/eclipse_state.cpp
          python/cxx/export.cpp
          python/cxx/group.cpp
          python/cxx/log.cpp
          python/cxx/parsecontext.cpp
          python/cxx/parser.cpp
          python/cxx/schedule.cpp
          python/cxx/summary_state.cpp
          python/cxx/table_manager.cpp
          python/cxx/unit_system.cpp
          python/cxx/well.cpp
          )

  if (OPM_ENABLE_EMBEDDED_PYTHON)
    set_source_files_properties(${PYTHON_CXX_SOURCE_FILES} PROPERTIES COMPILE_FLAGS -Wno-shadow)
    set_source_files_properties(src/opm/parser/eclipse/Python/PythonInterp.cpp PROPERTIES COMPILE_FLAGS -Wno-shadow)
    set_source_files_properties(src/opm/parser/eclipse/EclipseState/Schedule/Action/PyAction.cpp PROPERTIES COMPILE_FLAGS -Wno-shadow)
    list( APPEND MAIN_SOURCE_FILES src/opm/parser/eclipse/Python/PythonInterp.cpp ${PYTHON_CXX_SOURCE_FILES})
  endif()

  list( APPEND PYTHON_CXX_DEPENDS ${PYTHON_CXX_SOURCE_FILES}
    python/cxx/converters.hpp
    python/cxx/export.hpp)

  if(NOT cjson_FOUND)
    list(APPEND MAIN_SOURCE_FILES external/cjson/cJSON.c)
  endif()
endif()
if(ENABLE_ECL_OUTPUT)
  list( APPEND MAIN_SOURCE_FILES
          src/opm/io/eclipse/EclFile.cpp
          src/opm/io/eclipse/EclOutput.cpp
          src/opm/io/eclipse/EclUtil.cpp
          src/opm/io/eclipse/EGrid.cpp
          src/opm/io/eclipse/ERft.cpp
          src/opm/io/eclipse/ERst.cpp
          src/opm/io/eclipse/ESmry.cpp
          src/opm/io/eclipse/OutputStream.cpp
          src/opm/io/eclipse/rst/connection.cpp
          src/opm/io/eclipse/rst/header.cpp
          src/opm/io/eclipse/rst/segment.cpp
          src/opm/output/eclipse/AggregateActionxData.cpp
          src/opm/output/eclipse/AggregateConnectionData.cpp
          src/opm/output/eclipse/AggregateGroupData.cpp
          src/opm/output/eclipse/AggregateMSWData.cpp
          src/opm/output/eclipse/AggregateUDQData.cpp
          src/opm/output/eclipse/AggregateWellData.cpp
          src/opm/output/eclipse/CreateActionxDims.cpp
          src/opm/output/eclipse/CreateDoubHead.cpp
          src/opm/output/eclipse/CreateInteHead.cpp
          src/opm/output/eclipse/CreateLogiHead.cpp
          src/opm/output/eclipse/CreateUdqDims.cpp
          src/opm/output/eclipse/DoubHEAD.cpp
          src/opm/output/eclipse/EclipseGridInspector.cpp
          src/opm/output/eclipse/EclipseIO.cpp
          src/opm/output/eclipse/InteHEAD.cpp
          src/opm/output/eclipse/LinearisedOutputTable.cpp
          src/opm/output/eclipse/LoadRestart.cpp
          src/opm/output/eclipse/LogiHEAD.cpp
          src/opm/output/eclipse/RestartIO.cpp
          src/opm/output/eclipse/Summary.cpp
          src/opm/output/eclipse/Tables.cpp
          src/opm/output/eclipse/RegionCache.cpp
          src/opm/output/eclipse/RestartValue.cpp
          src/opm/output/eclipse/WriteInit.cpp
          src/opm/output/eclipse/WriteRFT.cpp
          src/opm/output/data/Solution.cpp
      )
endif()

list (APPEND TEST_SOURCE_FILES
      tests/test_ActiveGridCells.cpp
      tests/test_calculateCellVol.cpp
      tests/test_cmp.cpp
      tests/test_cubic.cpp
      tests/test_messagelimiter.cpp
      tests/test_nonuniformtablelinear.cpp
      tests/test_OpmLog.cpp
      tests/test_param.cpp
      tests/test_RootFinders.cpp
      tests/test_SimulationDataContainer.cpp
      tests/test_sparsevector.cpp
      tests/test_uniformtablelinear.cpp
)
if(ENABLE_ECL_INPUT)
  list(APPEND TEST_SOURCE_FILES
    tests/parser/ACTIONX.cpp
    tests/parser/ADDREGTests.cpp
    tests/parser/AquiferCTTests.cpp
    tests/parser/AquifetpTests.cpp
    tests/parser/AqudimsTests.cpp
    tests/parser/AquanconTests.cpp
    tests/parser/BoxTests.cpp
    tests/parser/ColumnSchemaTests.cpp
    tests/parser/ConnectionTests.cpp
    tests/parser/COMPSEGUnits.cpp
    tests/parser/CopyRegTests.cpp
    tests/parser/DeckValueTests.cpp
    tests/parser/DeckTests.cpp
    tests/parser/DynamicStateTests.cpp
    tests/parser/DynamicVectorTests.cpp
    tests/parser/Eclipse3DPropertiesTests.cpp
    tests/parser/EclipseGridTests.cpp
    tests/parser/EmbeddedPython.cpp
    tests/parser/EqualRegTests.cpp
    tests/parser/EventTests.cpp
    tests/parser/FaceDirTests.cpp
    tests/parser/FaultTests.cpp
    tests/parser/FieldPropsTests.cpp
    tests/parser/FoamTests.cpp
    tests/parser/FunctionalTests.cpp
    tests/parser/GeomodifierTests.cpp
    tests/parser/GridPropertyTests.cpp
    tests/parser/GroupTests.cpp
    tests/parser/InitConfigTest.cpp
    tests/parser/IOConfigTests.cpp
    tests/parser/MessageLimitTests.cpp
    tests/parser/MultiRegTests.cpp
    tests/parser/MultisegmentWellTests.cpp
    tests/parser/MULTREGTScannerTests.cpp
    tests/parser/OrderedMapTests.cpp
    tests/parser/ParseContextTests.cpp
    tests/parser/ParseContext_EXIT1.cpp
    tests/parser/ParseDATAWithDefault.cpp
    tests/parser/PYACTION.cpp
    tests/parser/PORVTests.cpp
    tests/parser/RawKeywordTests.cpp
    tests/parser/ResinsightTest.cpp
    tests/parser/RestartConfigTests.cpp
    tests/parser/RFTConfigTests.cpp
    tests/parser/RockTableTests.cpp
    tests/parser/RunspecTests.cpp
    tests/parser/SaltTableTests.cpp
    tests/parser/SatfuncPropertyInitializersTests.cpp
    tests/parser/ScheduleTests.cpp
    tests/parser/SectionTests.cpp
    tests/parser/SimpleTableTests.cpp
    tests/parser/SimulationConfigTest.cpp
    tests/parser/StarTokenTests.cpp
    tests/parser/StringTests.cpp
    tests/parser/SummaryConfigTests.cpp
    tests/parser/TabdimsTests.cpp
    tests/parser/TableColumnTests.cpp
    tests/parser/TableContainerTests.cpp
    tests/parser/TableManagerTests.cpp
    tests/parser/TableSchemaTests.cpp
    tests/parser/ThresholdPressureTest.cpp
    tests/parser/TimeMapTest.cpp
    tests/parser/TransMultTests.cpp
    tests/parser/TuningTests.cpp
    tests/parser/UDQTests.cpp
    tests/parser/UnitTests.cpp
    tests/parser/ValueTests.cpp
    tests/parser/WellSolventTests.cpp
    tests/parser/WellTracerTests.cpp
    tests/parser/WellTests.cpp
    tests/parser/WLIST.cpp
    tests/parser/WTEST.cpp)
endif()
if(ENABLE_ECL_OUTPUT)
  list (APPEND TEST_SOURCE_FILES
          tests/test_AggregateActionxData.cpp
          tests/test_AggregateWellData.cpp
          tests/test_AggregateGroupData.cpp
          tests/test_AggregateMSWData.cpp
          tests/test_AggregateConnectionData.cpp
          tests/test_AggregateUDQData.cpp
          tests/test_ArrayDimChecker.cpp
          tests/test_EclipseIO.cpp
          tests/test_DoubHEAD.cpp
          tests/test_InteHEAD.cpp
          tests/test_LinearisedOutputTable.cpp
          tests/test_LogiHEAD.cpp
          tests/test_OutputStream.cpp
          tests/test_regionCache.cpp
          tests/test_PaddedOutputString.cpp
          tests/test_Restart.cpp
          tests/test_RFT.cpp
          tests/test_Solution.cpp
          tests/test_Summary.cpp
          tests/test_Tables.cpp
          tests/test_Wells.cpp
          tests/test_WindowedArray.cpp
          tests/test_restartwellinfo.cpp
      )
endif()

list (APPEND TEST_DATA_FILES
      tests/testdata.param
)
if(ENABLE_ECL_OUTPUT)
  list (APPEND TEST_DATA_FILES
          tests/expect-wdims.chldg.err.out
          tests/expect-wdims.err.out
          tests/FIRST_SIM.DATA
          tests/FIRST_SIM_THPRES.DATA
          tests/summary_deck.DATA
          tests/group_group.DATA
          tests/testblackoilstate3.DATA
          tests/testrft.DATA
          tests/table_deck.DATA
          tests/summary_deck_non_constant_porosity.DATA
          tests/SUMMARY_EFF_FAC.DATA
          tests/SPE1CASE1.DATA
          tests/SPE1CASE1.SMSPEC
          tests/SPE1CASE1A.SMSPEC
          tests/SPE9_CP_PACKED.DATA
          tests/SOFR_TEST.DATA
          tests/UDQ_TEST_WCONPROD_IUAD-2.DATA
          tests/UDQ_ACTIONX_TEST1.DATA
          tests/include_example_pvt.txt
          tests/include_example_summary.txt
          tests/include_sgof.txt
          tests/include_swof.txt
          tests/include_grid_3x5x4.grdecl
      )
endif()

list (APPEND EXAMPLE_SOURCE_FILES
)
if(ENABLE_ECL_INPUT)
  list (APPEND TEST_DATA_FILES
    tests/ECLFILE.INIT
    tests/ECLFILE.FINIT
    tests/SPE1CASE1.EGRID
    tests/SPE1CASE1.RFT
    tests/SPE1_TESTCASE.UNRST
    tests/SPE1_TESTCASE.FUNRST
    tests/SPE1_TESTCASE.F0025
    tests/SPE1_TESTCASE.X0025
    tests/SPE1CASE1.UNSMRY
    tests/SPE1CASE1A.UNSMRY
    tests/SPE1CASE1_RST60.SMSPEC
    tests/SPE1CASE1_RST60.UNSMRY
  )
  list (APPEND EXAMPLE_SOURCE_FILES
    examples/opmi.cpp
    examples/opmpack.cpp
    examples/opmhash.cpp
  )
endif()

# programs listed here will not only be compiled, but also marked for
# installation
list (APPEND PROGRAM_SOURCE_FILES
)
if(ENABLE_ECL_INPUT)
  list (APPEND PROGRAM_SOURCE_FILES
    examples/opmi.cpp
    examples/opmpack.cpp
    examples/opmhash.cpp
  )
endif()

list( APPEND PUBLIC_HEADER_FILES
      opm/common/ErrorMacros.hpp
      opm/common/Exceptions.hpp
      opm/common/data/SimulationDataContainer.hpp
      opm/common/OpmLog/CounterLog.hpp
      opm/common/OpmLog/EclipsePRTLog.hpp
      opm/common/OpmLog/LogBackend.hpp
      opm/common/OpmLog/Logger.hpp
      opm/common/OpmLog/LogUtil.hpp
      opm/common/OpmLog/MessageFormatter.hpp
      opm/common/OpmLog/MessageLimiter.hpp
      opm/common/OpmLog/Location.hpp
      opm/common/OpmLog/OpmLog.hpp
      opm/common/OpmLog/StreamLog.hpp
      opm/common/OpmLog/TimerLog.hpp
      opm/common/utility/ActiveGridCells.hpp
      opm/common/utility/numeric/cmp.hpp
      opm/common/utility/platform_dependent/disable_warnings.h
      opm/common/utility/platform_dependent/reenable_warnings.h
      opm/common/utility/numeric/blas_lapack.h
      opm/common/utility/numeric/buildUniformMonotoneTable.hpp
      opm/common/utility/numeric/linearInterpolation.hpp
      opm/common/utility/numeric/MonotCubicInterpolator.hpp
      opm/common/utility/numeric/NonuniformTableLinear.hpp
      opm/common/utility/numeric/RootFinders.hpp
      opm/common/utility/numeric/SparseVector.hpp
      opm/common/utility/numeric/UniformTableLinear.hpp
      opm/common/utility/parameters/ParameterGroup.hpp
      opm/common/utility/parameters/ParameterGroup_impl.hpp
      opm/common/utility/parameters/Parameter.hpp
      opm/common/utility/parameters/ParameterMapItem.hpp
      opm/common/utility/parameters/ParameterRequirement.hpp
      opm/common/utility/parameters/ParameterStrings.hpp
      opm/common/utility/parameters/ParameterTools.hpp
      opm/common/utility/numeric/calculateCellVol.hpp
      opm/common/utility/TimeService.hpp
)
if(ENABLE_ECL_INPUT)
  list(APPEND PUBLIC_HEADER_FILES
       opm/json/JsonObject.hpp
       opm/parser/eclipse/Utility/Stringview.hpp
       opm/parser/eclipse/Utility/Functional.hpp
       opm/parser/eclipse/Utility/Typetools.hpp
       opm/parser/eclipse/Utility/String.hpp
       opm/parser/eclipse/Generator/KeywordGenerator.hpp
       opm/parser/eclipse/Generator/KeywordLoader.hpp
       opm/parser/eclipse/Units/UnitSystem.hpp
       opm/parser/eclipse/Units/Units.hpp
       opm/parser/eclipse/Units/Dimension.hpp
       opm/parser/eclipse/Parser/ErrorGuard.hpp
       opm/parser/eclipse/Parser/ParserItem.hpp
       opm/parser/eclipse/Parser/Parser.hpp
       opm/parser/eclipse/Parser/ParserRecord.hpp
       opm/parser/eclipse/Parser/ParserKeyword.hpp
       opm/parser/eclipse/Parser/InputErrorAction.hpp
       opm/parser/eclipse/Parser/ParserEnums.hpp
       opm/parser/eclipse/Parser/ParseContext.hpp
       opm/parser/eclipse/Parser/ParserConst.hpp
       opm/parser/eclipse/EclipseState/InitConfig/InitConfig.hpp
       opm/parser/eclipse/EclipseState/InitConfig/Equil.hpp
       opm/parser/eclipse/EclipseState/InitConfig/FoamConfig.hpp
       opm/parser/eclipse/EclipseState/Util/Value.hpp
       opm/parser/eclipse/EclipseState/Util/IOrderSet.hpp
       opm/parser/eclipse/EclipseState/Util/OrderedMap.hpp
       opm/parser/eclipse/EclipseState/SummaryConfig/SummaryConfig.hpp
       opm/parser/eclipse/EclipseState/Edit/EDITNNC.hpp
       opm/parser/eclipse/EclipseState/Grid/GridDims.hpp
       opm/parser/eclipse/EclipseState/Grid/TransMult.hpp
       opm/parser/eclipse/EclipseState/Grid/GridProperties.hpp
       opm/parser/eclipse/EclipseState/Grid/PinchMode.hpp
       opm/parser/eclipse/EclipseState/Grid/MULTREGTScanner.hpp
       opm/parser/eclipse/EclipseState/Grid/FaultCollection.hpp
       opm/parser/eclipse/EclipseState/Grid/SatfuncPropertyInitializers.hpp
       opm/parser/eclipse/EclipseState/Grid/Fault.hpp
       opm/parser/eclipse/EclipseState/Grid/Box.hpp
       opm/parser/eclipse/EclipseState/Grid/FieldPropsManager.hpp
       opm/parser/eclipse/EclipseState/Grid/GridProperty.hpp
       opm/parser/eclipse/EclipseState/Grid/FaultFace.hpp
       opm/parser/eclipse/EclipseState/Grid/NNC.hpp
       opm/parser/eclipse/EclipseState/Grid/EclipseGrid.hpp
       opm/parser/eclipse/EclipseState/Grid/BoxManager.hpp
       opm/parser/eclipse/EclipseState/Grid/FaceDir.hpp
       opm/parser/eclipse/EclipseState/Grid/MinpvMode.hpp
       opm/parser/eclipse/EclipseState/Eclipse3DProperties.hpp
       opm/parser/eclipse/EclipseState/EndpointScaling.hpp
       opm/parser/eclipse/EclipseState/Tables/SimpleTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PolyInjTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PdvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/TlpmixpaTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvdgTable.hpp
       opm/parser/eclipse/EclipseState/Tables/MsfnTable.hpp
       opm/parser/eclipse/EclipseState/Tables/GasvisctTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Regdims.hpp
       opm/parser/eclipse/EclipseState/Tables/Eqldims.hpp
       opm/parser/eclipse/EclipseState/Tables/SpecrockTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvtwsaltTable.hpp
       opm/parser/eclipse/EclipseState/Tables/BrineDensityTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlydhflfTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlymwinjTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlyshlogTable.hpp
       opm/parser/eclipse/EclipseState/Tables/RsvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SkprwatTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SkprpolyTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SpecheatTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SgcwmisTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Sof2Table.hpp
       opm/parser/eclipse/EclipseState/Tables/TableManager.hpp
       opm/parser/eclipse/EclipseState/Tables/SwfnTable.hpp
       opm/parser/eclipse/EclipseState/Tables/EnptvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SwofTable.hpp
       opm/parser/eclipse/EclipseState/Tables/FlatTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Aqudims.hpp
       opm/parser/eclipse/EclipseState/Tables/JFunc.hpp
       opm/parser/eclipse/EclipseState/Tables/TableIndex.hpp
       opm/parser/eclipse/EclipseState/Tables/PvtgTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Tabdims.hpp
       opm/parser/eclipse/EclipseState/Tables/TableSchema.hpp
       opm/parser/eclipse/EclipseState/Tables/RocktabTable.hpp
       opm/parser/eclipse/EclipseState/Tables/EnkrvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlyrockTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvtxTable.hpp
       opm/parser/eclipse/EclipseState/Tables/WatvisctTable.hpp
       opm/parser/eclipse/EclipseState/Tables/TableEnums.hpp
       opm/parser/eclipse/EclipseState/Tables/RvvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/TableContainer.hpp
       opm/parser/eclipse/EclipseState/Tables/AqutabTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlyadsTable.hpp
       opm/parser/eclipse/EclipseState/Tables/FoamadsTable.hpp
       opm/parser/eclipse/EclipseState/Tables/FoammobTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PbvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SorwmisTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlymaxTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PlyviscTable.hpp
       opm/parser/eclipse/EclipseState/Tables/TableColumn.hpp
       opm/parser/eclipse/EclipseState/Tables/SsfnTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvdoTable.hpp
       opm/parser/eclipse/EclipseState/Tables/OilvisctTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SgfnTable.hpp
       opm/parser/eclipse/EclipseState/Tables/MiscTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SgwfnTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvdsTable.hpp
       opm/parser/eclipse/EclipseState/Tables/PvtoTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Rock2dTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Rock2dtrTable.hpp
       opm/parser/eclipse/EclipseState/Tables/RockwnodTable.hpp
       opm/parser/eclipse/EclipseState/Tables/OverburdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/ColumnSchema.hpp
       opm/parser/eclipse/EclipseState/Tables/PmiscTable.hpp
       opm/parser/eclipse/EclipseState/Tables/RtempvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/SlgofTable.hpp
       opm/parser/eclipse/EclipseState/Tables/ImptvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/ImkrvdTable.hpp
       opm/parser/eclipse/EclipseState/Tables/Sof3Table.hpp
       opm/parser/eclipse/EclipseState/Tables/SgofTable.hpp
       opm/parser/eclipse/EclipseState/EclipseState.hpp
       opm/parser/eclipse/EclipseState/EclipseConfig.hpp
       opm/parser/eclipse/EclipseState/Aquancon.hpp
       opm/parser/eclipse/EclipseState/AquiferCT.hpp
       opm/parser/eclipse/EclipseState/Aquifetp.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ActionAST.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ActionContext.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ActionResult.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ActionValue.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/Actdims.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/Actions.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ActionX.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/Condition.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/ASTNode.hpp
       opm/parser/eclipse/EclipseState/Schedule/Action/PyAction.hpp
       opm/parser/eclipse/EclipseState/Schedule/ArrayDimChecker.hpp
       opm/parser/eclipse/EclipseState/Schedule/TimeMap.hpp
       opm/parser/eclipse/EclipseState/Schedule/VFPInjTable.hpp
       opm/parser/eclipse/EclipseState/Schedule/VFPProdTable.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/Connection.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/ProductionControls.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/InjectionControls.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/Well.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WList.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WListManager.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellEconProductionLimits.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellFoamProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellBrineProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellInjectionProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellPolymerProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellProductionProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellTracerProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellTestConfig.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellTestState.hpp
       opm/parser/eclipse/EclipseState/Schedule/Well/WellConnections.hpp
       opm/parser/eclipse/EclipseState/Schedule/DynamicVector.hpp
       opm/parser/eclipse/EclipseState/Schedule/SummaryState.hpp
       opm/parser/eclipse/EclipseState/Schedule/RFTConfig.hpp
       opm/parser/eclipse/EclipseState/Schedule/Schedule.hpp
       opm/parser/eclipse/EclipseState/Schedule/Tuning.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GTNode.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/Group.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GuideRate.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GConSale.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GConSump.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GuideRateConfig.hpp
       opm/parser/eclipse/EclipseState/Schedule/Group/GuideRateModel.hpp
       opm/parser/eclipse/EclipseState/Schedule/MessageLimits.hpp
       opm/parser/eclipse/EclipseState/Schedule/Events.hpp
       opm/parser/eclipse/EclipseState/Schedule/OilVaporizationProperties.hpp
       opm/parser/eclipse/EclipseState/Schedule/DynamicState.hpp
       opm/parser/eclipse/EclipseState/Schedule/MSW/Segment.hpp
       opm/parser/eclipse/EclipseState/Schedule/MSW/WellSegments.hpp
       opm/parser/eclipse/EclipseState/Schedule/MSW/updatingConnectionsWithSegments.hpp
       opm/parser/eclipse/EclipseState/Schedule/MSW/SpiralICD.hpp
       opm/parser/eclipse/EclipseState/SimulationConfig/ThresholdPressure.hpp
       opm/parser/eclipse/EclipseState/SimulationConfig/SimulationConfig.hpp
       opm/parser/eclipse/EclipseState/Schedule/MSW/Valve.hpp
       opm/parser/eclipse/EclipseState/IOConfig/RestartConfig.hpp
       opm/parser/eclipse/EclipseState/IOConfig/IOConfig.hpp
       opm/parser/eclipse/EclipseState/checkDeck.hpp
       opm/parser/eclipse/EclipseState/Runspec.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQAssign.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQASTNode.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQDefine.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQContext.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQConfig.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQEnums.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQParams.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQInput.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQActive.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQSet.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQFunction.hpp
       opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQFunctionTable.hpp
       opm/parser/eclipse/Deck/DeckItem.hpp
       opm/parser/eclipse/Deck/Deck.hpp
       opm/parser/eclipse/Deck/DeckSection.hpp
       opm/parser/eclipse/Deck/DeckOutput.hpp
       opm/parser/eclipse/Deck/DeckValue.hpp
       opm/parser/eclipse/Deck/DeckKeyword.hpp
       opm/parser/eclipse/Deck/DeckRecord.hpp
       opm/parser/eclipse/Deck/UDAValue.hpp
       opm/parser/eclipse/Deck/value_status.hpp
       opm/parser/eclipse/Python/Python.hpp)
endif()
if(ENABLE_ECL_OUTPUT)
  list(APPEND PUBLIC_HEADER_FILES
        opm/io/eclipse/EclFile.hpp
        opm/io/eclipse/EclIOdata.hpp
        opm/io/eclipse/EclOutput.hpp
        opm/io/eclipse/EclUtil.hpp
        opm/io/eclipse/EGrid.hpp
        opm/io/eclipse/ERft.hpp
        opm/io/eclipse/ERst.hpp
        opm/io/eclipse/ESmry.hpp
        opm/io/eclipse/PaddedOutputString.hpp
        opm/io/eclipse/OutputStream.hpp
        opm/io/eclipse/rst/connection.hpp
        opm/io/eclipse/rst/header.hpp
        opm/io/eclipse/rst/segment.hpp
        opm/output/data/Aquifer.hpp
        opm/output/data/Cells.hpp
        opm/output/data/Solution.hpp
        opm/output/data/Wells.hpp
        opm/output/eclipse/VectorItems/aquifer.hpp
        opm/output/eclipse/VectorItems/connection.hpp
        opm/output/eclipse/VectorItems/group.hpp
        opm/output/eclipse/VectorItems/intehead.hpp
        opm/output/eclipse/VectorItems/logihead.hpp
        opm/output/eclipse/VectorItems/msw.hpp
        opm/output/eclipse/VectorItems/tabdims.hpp
        opm/output/eclipse/VectorItems/well.hpp
        opm/output/eclipse/AggregateActionxData.hpp
        opm/output/eclipse/AggregateGroupData.hpp
        opm/output/eclipse/AggregateConnectionData.hpp
        opm/output/eclipse/AggregateMSWData.hpp
        opm/output/eclipse/AggregateUDQData.hpp
        opm/output/eclipse/AggregateWellData.hpp
        opm/output/eclipse/DoubHEAD.hpp
        opm/output/eclipse/EclipseGridInspector.hpp
        opm/output/eclipse/EclipseIO.hpp
        opm/output/eclipse/EclipseIOUtil.hpp
        opm/output/eclipse/InteHEAD.hpp
        opm/output/eclipse/LinearisedOutputTable.hpp
        opm/output/eclipse/LogiHEAD.hpp
        opm/output/eclipse/RegionCache.hpp
        opm/output/eclipse/RestartIO.hpp
        opm/output/eclipse/RestartValue.hpp
        opm/output/eclipse/Summary.hpp
        opm/output/eclipse/Tables.hpp
        opm/output/eclipse/WindowedArray.hpp
        opm/output/eclipse/WriteInit.hpp
        opm/output/eclipse/WriteRFT.hpp
        opm/output/eclipse/WriteRestartHelpers.hpp
        opm/output/OutputWriter.hpp
        )
endif()
