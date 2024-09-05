{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  packages = with pkgs; [
    # for development and building
    clang
    clang-tools
    gnumake
    gdb
    valgrind

    # deps
    cereal
  ];

}
