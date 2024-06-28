{
  description = "Flake for task cli app. Contains deps and dev tools.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }: 
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in {
      devShell = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          clang
          jsoncpp
          clang-tools
          gnumake
          # cmake
          gdb
          valgrind
          # zstd  # for uncompressing core files
        ];
      };
    }
  );
}
