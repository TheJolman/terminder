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
    rev = "v2.0.0";
    sha256 = "sha256-zm0L3Is/vviMb4N+kFcXkvaRMFb1udz/5ES6sjdhBGg=";
  };

  buildInputs = [ cereal clang ];
  
  buildPhase = "make";

  installPhase = ''
    runHook preInstall
    mkdir -p $out/bin
    cp build/terminder $out/bin
    runHook postInstall
  '';
}
