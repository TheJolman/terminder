{
  description = "Terminder - A todo app";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
    pre-commit-hooks.url = "github:cachix/pre-commit-hooks.nix";
  };

  outputs = { self, nixpkgs, flake-utils, pre-commit-hooks }:
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

        checks = {
          pre-commit-check = pre-commit-hooks.lib.${system}.run {
            src = ./.;
            hooks = {
              clang-format = {
                enable = true;
                files = "\\.(cpp|hpp)$";
              };
            };
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang
            clang-tools
            cereal
            valgrind
            cppcheck
            bear
          ];

          shellHook = ''
            ${self.checks.${system}.pre-commit-check}

            if ! test -f "compile_commands.json"; then
              make clean && bear -- make
            fi

          '';

        };
      }
    );
}
