$TEST_DIR = "D:\OneDrive_VinUni\OneDrive - VINACADEMY LLC\CourseData\Computer Organization\Project_RISCV\COMP2020_RISCV_Interpreter\test_cases_104"
$EXECUTABLE = "D:\OneDrive_VinUni\OneDrive - VINACADEMY LLC\CourseData\Computer Organization\Project_RISCV\COMP2020_RISCV_Interpreter\riscv_interpreter.exe"

# Kiểm tra xem file thực thi có tồn tại không
if (-Not (Test-Path $EXECUTABLE)) {
    Write-Host "Error: $EXECUTABLE not found. Compile it first."
    exit 1
}

# Kiểm tra nếu thư mục test không tồn tại
if (-Not (Test-Path $TEST_DIR)) {
    Write-Host "Error: $TEST_DIR not found."
    exit 1
}

# Duyệt qua các file *_input.txt trong thư mục test
Get-ChildItem "$TEST_DIR\*_input.txt" | ForEach-Object {
    $INPUT_FILE = $_.FullName
    $BASE_NAME = $_.BaseName
    Write-Host $BASE_NAME
    # Loại bỏ hậu tố "_input"
    $BASE_NAME = $BASE_NAME -replace '_input$', ''

    $EXPECTED_FILE = "$TEST_DIR\$BASE_NAME`_expected.txt"
    $ACTUAL_FILE = "$TEST_DIR\$BASE_NAME`_actual.txt"

   #  Kiểm tra xem file mong đợi có tồn tại không
    if (-Not (Test-Path $EXPECTED_FILE)) {
       Write-Host "Error: $EXPECTED_FILE not found. Skipping test: $BASE_NAME"
       return
    }

    # Hiển thị thông tin kiểm tra test
    Write-Host "Running test: $BASE_NAME"

    # Chạy chương trình và lưu kết quả thực tế vào file
    Get-Content $INPUT_FILE | & $EXECUTABLE -d | Where-Object { $_ -notmatch "^\(DEBUG:" } > $ACTUAL_FILE  

    # Get-Content $INPUT_FILE | & $EXECUTABLE -d

    # So sánh file kết quả thực tế với kết quả mong đợi
    if (Compare-Object (Get-Content $ACTUAL_FILE) (Get-Content $EXPECTED_FILE)) {
        Write-Host "Test ${BASE_NAME}: FAILED"
        Write-Host "Differences:"
        Compare-Object (Get-Content $ACTUAL_FILE) (Get-Content $EXPECTED_FILE)
    } else {
        Write-Host "Test ${BASE_NAME}: PASSED"
    }
}
