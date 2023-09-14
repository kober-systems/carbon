let
  sources = import ./nix/sources.nix;
in
{ pkgs ? import sources.nixpkgs {} }:

  pkgs.mkShell {
    buildInputs = with pkgs; [
      platformio
      gcc

      teensy-loader-cli

      # language server etc
      clang-tools
    ];
}

