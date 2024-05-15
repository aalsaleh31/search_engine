
# Search Engine Project

This project returns a ranked list of files from a specified directory based on the user's search query.

## Usage Instructions

To compile and run the program, follow these steps and respond to the prompts:

```sh
$ make
$ ./search
```
*(If `make` command fails, run `make clean` and try again.)*

### Example Workflow:

1. **Compile and execute the program:**
   ```sh
   $ make
   $ ./search
   ```
2. **Specify the number of buckets:**
   ```sh
   How many buckets?
   > 10
   ```
3. **Provide a filename search string:**
   ```sh
   Enter a filename search string:
   > docs/*.txt
   ```
4. **Input your search query (or 'X' to exit):**
   ```sh
   Enter a search query (X to exit):
   > the computers are not working
   ```
5. **The program will return ranked files:**
   ```sh
   docs/D1.txt
   docs/D2.txt
   ```
6. **To exit, enter 'X':**
   ```sh
   Enter a search query (X to exit):
   > X
   ```

### Note on Search Scores:

After each search query, the calculated scores used for ranking the files are appended to `search_scores.txt` in the following format:

```
docs/D1.txt: 1.205356
docs/D2.txt: 0.623956
docs/D3.txt: 0.000000
```

Ensure to check this file for detailed scores after performing searches.
