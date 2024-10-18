{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  packages = with pkgs; [
    clang
    clang-tools
    lldb
    gnumake
    bear
    valgrind
    cereal
  ];

}
