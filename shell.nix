{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Core Build Tools
    cmake
    ninja

    # Documentation Tools
    doxygen
    graphviz

    # Python environment for MkDocs
    (python3.withPackages (ps: with ps; [
      mkdocs-material
    ]))

    (pkgs.stdenv.mkDerivation {
      pname = "doxybook2";
      version = "1.5.0";
      src = pkgs.fetchurl {
        url = "https://github.com/matusnovak/doxybook2/releases/download/v1.5.0/doxybook2-linux-amd64-v1.5.0.zip";
        sha256 = "0havpdg1in6bil45cpl0y99hrapnby322qk0llwq2gmbnxa07f9z";
      };
      nativeBuildInputs = [ pkgs.unzip ];
      unpackPhase = "unzip $src";
      installPhase = ''
        mkdir -p $out/bin
        # This looks for the file 'doxybook2' recursively and copies it to the output
        find . -type f -name "doxybook2" -exec cp {} $out/bin/doxybook2 \;
        chmod +x $out/bin/doxybook2
      '';
    })
  ];

  shellHook = ''
    echo "C++ Documentation Environment Loaded"
    echo "Run 'cmake -S . -B build' then 'cmake --build build --target docs'"
  '';
}
