#  Brother John real-time melody player for singleboard ARM computer MD-407

This joint project consists of a concurrent program that plays the melody of Brother John in the AUX port of the board.
The player can be controlled by typing certain letters and a numeric value into a io console trough
USB to control the player. TinyTimber is a kernel made in Luleå University and is written in C that enables concurrency
and easy access and handling of the IO ports on the ARM board. So all TinyTimber files and drivers are not made by us.

To build any of the projects then the ARM gcc & g++ toolchain is needed along with a way to send
the compiled software to your device such as GDB or by cable. Run `make *.mk` to compile and make the program.

There is also documentation about the MD-407 and the components used with it. The MD-407 and some
components are created by chalmers so some of the documentation is in Swedish. The documents can be
found in the `docs` folder.
