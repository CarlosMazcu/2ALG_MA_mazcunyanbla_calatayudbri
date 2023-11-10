cls
@echo off
IF EXIST ..\build\.vs rmdir /s /q ..\build\.vs
IF EXIST ..\build\PR00_TestMemoryManager rmdir /s /q ..\build\PR00_TestMemoryManager
IF EXIST ..\build\PR01_MemoryNode rmdir /s /q ..\build\PR01_MemoryNode
IF EXIST ..\build\PR02_Vector rmdir /s /q ..\build\PR02_Vector
IF EXIST ..\build\PR03_CircularVector rmdir /s /q ..\build\PR03_CircularVector
IF EXIST ..\build\PR04_MovableVector rmdir /s /q ..\build\PR04_MovableVector
IF EXIST ..\build\PR05_List rmdir /s /q ..\build\PR05_List
IF EXIST ..\build\PR06_DLList rmdir /s /q ..\build\PR06_DLList
IF EXIST ..\build\PR07_CircularList rmdir /s /q ..\build\PR07_CircularList
IF EXIST ..\build\PR08_CircularDLList rmdir /s /q ..\build\PR08_CircularDLList
IF EXIST ..\build\PR09_Stack rmdir /s /q ..\build\PR09_Stack
IF EXIST ..\build\PR10_Queue rmdir /s /q ..\build\PR10_Queue
IF EXIST ..\build\PR11_Logger rmdir /s /q ..\build\PR11_Logger
IF EXIST ..\build\PR12_Comparative rmdir /s /q ..\build\PR12_Comparative
IF EXIST ..\build\PR13_SortingAlgorithms rmdir /s /q ..\build\PR13_SortingAlgorithms
IF EXIST ..\bin\debug rmdir /s /q ..\bin\debug
IF EXIST ..\bin\release rmdir /s /q ..\bin\release
IF EXIST ..\build rmdir /s /q ..\build
IF EXIST ..\bin rmdir /s /q ..\bin

IF EXIST ..\build\DS_ALG_AI1vs2019.sln del /F ..\build\DS_ALG_AI1vs2019.sln
IF EXIST ..\build\DS_ALG_AI1vs2022.sln del /F ..\build\DS_ALG_AI1vs2022.sln
