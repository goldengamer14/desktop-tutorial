from openpyxl import load_workbook

class ExcelReader:
    def __init__(self, file_path, **kwargs):
        self.file_path = file_path
        self.workbook = load_workbook(filename=file_path, **kwargs)
        self.sheet = self.workbook.active

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.workbook:
            self.workbook.close()
            print("Workbook closed.")
