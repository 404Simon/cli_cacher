# CLI Cache

Simple cache for CLI applications. Hashes the command and arguments to a file path and stores them in a specified directory.
If the command failes, e.g. because of a network error, the cache is used to return the last successful result.

## Compile

```bash
g++ -std=c++17 -o cli-cache cli-cache.cpp
```

## Usage

```bash
./cli-cache <cache_dir> <command> <args>
```

For example:

```bash
./cli-cache /tmp/ curl https://example.com
```

For ease of use, you could add an alias to your `.zshrc` or `.bashrc`:

```bash
alias ccurl='./cli-cache /my/cache/dir/ curl'
```

Then you can use it like this:

```bash
ccurl https://example.com
```

Which is nice :)

