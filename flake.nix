{
  description = "A flake for developing in the Tispy.h project (please make changes as necessary)";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    home-manager.url = "github:nix-community/home-manager";
    home-manager.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, home-manager } @ inputs:
    let
      system = "x86_64-linux";
      userName = "miguel";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      homeConfigurations.${userName} = home-manager.lib.homeManager {
        inherit pkgs;
        modules = [
          ./home.nix
        ];
      };
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          zsh
          git
          clang-tools
        ];
        shellHook = ''
        export SHELL="${pkgs.zsh}/bin/zsh"
        zsh -c "tmux"
        '';
      };
    };
}
