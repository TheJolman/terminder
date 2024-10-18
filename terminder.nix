{
  stdenv,
  fetchFromGitHub,
  cereal,
  clang,
}:

stdenv.mkDerivation {
  pname = "terminder";
  version = "v2.0";

  src = fetchFromGitHub {
    owner = "TheJolman";
    repo = "Terminder";
    rev = "v2.0";
    sha256 = "sha256-53tKzEe91A9ChTtjwrXfDQBKDQxsP2MTe9A5X7FxjGs=";
  };

  buildInputs = [ cereal clang ];
}
