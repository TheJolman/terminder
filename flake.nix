{
  description = "Terminder - A todo app in cli form";

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
        packages.default = pkgs.callPackage ./terminder.nix { };

        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang
            cereal
          ];
        };
      }
    );
}
