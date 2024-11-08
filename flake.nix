{
  description = "Terminder - A todo app";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {

        packages.default = pkgs.stdenv.mkDerivation {
          name = "terminder";
          src = ./.;

          buildInputs = with pkgs; [
            clang
            cereal
          ];

          buildPhase = ''
            make
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp build/terminder $out/bin
          '';

        };

        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang
            cereal
            valgrind
          ];
        };
      }
    );
}
