{
  description = "Terminder - A todo app";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "terminder";
          src = ./.;

          buildInputs = with pkgs; [
            cereal
            cli11
          ];

          installPhase = "make install prefix=$out";
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            llvmPackages_20.libcxxClang
            llvmPackages_20.clang-tools
            llvmPackages_20.lldb
            cereal
            cli11
            valgrind
            cppcheck
            bear
          ];

          CXX = "clang++";

          shellHook = ''
            if ! test -f "compile_commands.json"; then
              make clean && bear -- make
            fi

            export CXX="clang++"
            export PATH="$PWD/build:$PATH"
          '';
        };
      }
    );
}
