# Neox

Neox is a WebKit based HTML CSS JS To Executable converter. It's designed to make HTML, CSS, and JS be aviable on desktop applications.

## Prequisites:

- GCC
- Webkit2
- GTK+3

1. Install Neox:

```bash
git clone https://github.com/voltageddebunked/neox.git
cd neox
mkdir build
cd build
cmake ..
make
```

Then, export the Neox executable into your PATH.

## Usage

To initialize a new project, run:

```bash
neox init
```

Then, after you're done building your project, run:

```bash
neox build
```

And now, your app will be built. You can specify the Identifier, app name, css, js and html file locations.

## License

[GPL-3.0](https://github.com/voltageddebunked/neox/blob/master/LICENSE)

## Disclaimer

This project is not affiliated with Apple or WebKit. It's just a personal project to convert HTML, CSS, and JS to desktop applications.
