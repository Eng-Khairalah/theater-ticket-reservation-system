"""Simple end-to-end check: successful booking, persistence, and duplicate rejection."""
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as temp:
    binary = pathlib.Path(temp) / "theater"
    subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Wpedantic", "-Werror", str(ROOT / "src/main.c"), "-o", str(binary)], check=True)
    first = subprocess.run([str(binary)], input="1\n2\n5\n", text=True, capture_output=True, cwd=temp, check=True)
    assert "Reservation confirmed!" in first.stdout
    assert (pathlib.Path(temp) / "seats.txt").read_text().splitlines()[1] == "1"
    second = subprocess.run([str(binary)], input="1\n2\n5\n", text=True, capture_output=True, cwd=temp, check=True)
    assert "already reserved" in second.stdout
    third = subprocess.run([str(binary)], input="abc\n4\n5\n", text=True, capture_output=True, cwd=temp, check=True)
    assert "valid number" in third.stdout
    assert "2 [X]" in third.stdout
print("All smoke tests passed.")
