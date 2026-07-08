## Project Structure

| Path | Description |
|---|---|
| `src/structures/heap.h/.cpp` | From-scratch min-heap. Built once per day, queried for the least-congested slot within a time range. |
| `src/structures/hashmap.h/.cpp` | From-scratch hash map. Keyed on `(day, hour)`, uses chaining, scans matching buckets for the min. |
| `src/data/GymRecord.h` | Shared struct definition (timestamp, day, hour, temperature, holiday flag, headcount). Everything else depends on this. |
| `src/data/loader.h/.cpp` | Reads the CSV dataset into a `vector<GymRecord>` for both structures to consume. |
| `src/bench/bench.h/.cpp` | Benchmarking harness (`<chrono>`). Times heap vs. hash map across dataset sizes and range widths. |
| `src/gui/MainWindow.h/.cpp` | Qt interface — input page (filters) and results page. Calls into `structures/` and `data/` directly. |
| `resources/` | Non-code assets — icons, `.qss` stylesheets, sample data files. |
| `CMakeLists.txt` | Build configuration. Tells CMake what to compile and where to find Qt. |
| `generator.py` | One-time offline script that pads the Kaggle dataset to 100k+ rows. Not compiled, not part of the app. |
| `.gitignore` | Keeps build artifacts (`cmake-build-debug/`, `.idea/`, `*.o`) out of version control. |

**Dependency direction:** `gui` and `bench` depend on `structures` and `data` — never the other way around. Keeps structures/data independently testable without needing the GUI built.
