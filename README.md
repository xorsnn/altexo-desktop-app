## Synopsis

Altexo holographic chat application

## Code Example

Show what the library does as concisely as possible, developers should be able to figure out **how** your project solves their problem by looking at the code example. Make sure the API you are showing off is obvious, and that your code is short and concise.

## Motivation

A short description of the motivation behind the creation and maintenance of the project. This should explain **why** the project exists.

## Installation
MAC:
- webrrtc:
in:
- webrtc/src/chromium/src/build/common.gypi

RTTI - (typeinfo link problem)
-          'GCC_ENABLE_CPP_RTTI': 'NO',              # -fno-rtti
+          'GCC_ENABLE_CPP_RTTI': 'YES',             # -fno-rtti


undefined warning workaround
-          'GCC_TREAT_WARNINGS_AS_ERRORS': 'YES',    # -Werror
+          'GCC_TREAT_WARNINGS_AS_ERRORS': 'NO',     # -Werror

STDLIB flag (std::__1:: vs std:: symbols mismatch at link time)
+              'CLANG_CXX_LIBRARY':           'libc++', # -stdlib=libc++


## API Reference

Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.

## Tests

Describe and show how to run the tests with code examples.

## Contributors

Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.

## License

A short snippet describing the license (MIT, Apache, etc.)
