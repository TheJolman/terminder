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

          shellHook = ''
            if ! test -f "build/compile_commands.json"; then
              echo "Warning: compile_commands.json not found. Run `make setup` on your first build!"
            fi

            export CXX="clang++"
            export BUILD_TYPE="debug"
            export PATH="$PWD/build:$PATH"
          '';
        };
      }
    );
}
