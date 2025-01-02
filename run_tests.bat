@echo off
setlocal

:: Thư mục chứa các file test
set TEST_DIR=D:\OneDrive_VinUni\OneDrive - VINACADEMY LLC\CourseData\Computer Organization\Project_RISCV\COMP2020_RISCV_Interpreter\tests
:: Đường dẫn đến file thực thi
set EXECUTABLE=D:\OneDrive_VinUni\OneDrive - VINACADEMY LLC\CourseData\Computer Organization\Project_RISCV\COMP2020_RISCV_Interpreter\riscv_interpreter.exe

:: Kiểm tra xem file thực thi có tồn tại không
if not exist "%EXECUTABLE%" (
    echo Error: %EXECUTABLE% not found. Compile it first.
    exit /b 1
)

:: Kiểm tra nếu thư mục test không tồn tại
if not exist "%TEST_DIR%" (
    echo Error: %TEST_DIR% not found test dir.
    exit /b 1
)

:: Duyệt qua các file *_input.txt trong thư mục test
for %%I in (%TEST_DIR%\*_input.txt) do (
    set INPUT_FILE=%%I
    set BASE_NAME=%%~nI
    set EXPECTED_FILE=%TEST_DIR%\%BASE_NAME%_expected.txt
    set ACTUAL_FILE=%TEST_DIR%\%BASE_NAME%_actual.txt

    :: Kiểm tra xem file mong đợi có tồn tại không
    if not exist "%EXPECTED_FILE%" (
        echo Error: %EXPECTED_FILE% not found.
        echo Skipping test: %BASE_NAME%
        continue
    )

    :: Chạy chương trình và lưu kết quả thực tế vào file
    echo Running test: %BASE_NAME%
    "%EXECUTABLE%" -d < "%INPUT_FILE%" 2> "%ACTUAL_FILE%"

    :: So sánh file kết quả thực tế với kết quả mong đợi
    fc /b "%ACTUAL_FILE%" "%EXPECTED_FILE%" > nul
    if errorlevel 1 (
        echo Test %BASE_NAME%: FAILED
        echo Differences:
        fc "%ACTUAL_FILE%" "%EXPECTED_FILE%"
    ) else (
        echo Test %BASE_NAME%: PASSED
    )
)

endlocal
