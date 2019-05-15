# Automagicaly

automagicaly - a python3 script for building skiftOS.

## Synopsis

```sh

automagicaly [ACTION] [TARGET]

```

## Actions

### `run` - start the kernel in qemu

This action will:
   1. build all targets
   2. generate a iso file
   3. start qemu  

```sh

./automagicaly run

```

### `build` - Build a target and it's dependancies

```sh

./automagicaly build [TARGET]

```

### `clean` - Delete all build file of the target

```sh

./automagicaly clear [TARGET]

```

### `rebuild` - Clean and build a target

```sh

./automagicaly rebuild [TARGET]

```

### The `-all` modifier

Append `-all` to apply an *action* to all targets.

```sh

./automagicaly build-all

```

## Exemples

```sh
# Run the operation system in qemu
./automagicaly run

# Show help
./automagicaly help

# Build a target
./automagicaly build skift.hjert.kernel

# Build everythings
./automagicaly build-all

# For a clean build (release)
./automagicaly rebuild-all
```