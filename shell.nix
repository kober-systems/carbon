let
  sources = import ./nix/sources.nix;
in
{ pkgs ? import sources.nixpkgs {} }:

  pkgs.mkShell {
    buildInputs = with pkgs; [
      platformio
      gcc

      teensy-loader-cli
      renode

      # language server etc
      clang-tools

      pkgs.zstd # needed for platformio as it does link dynamically against local libs
    ];

  shellHook = ''
    export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [ pkgs.zstd ]}''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
  '';
}

