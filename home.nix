{ config, pkgs, ... }:
{
  zshrcSrc = ".config/zsh/zshrc";
  home.file.".zshrc".source = "${config.home.homeDirectory}/${zshSrc}";
  home.file.".zshenv".source = "${config.home.homeDirectory}/.zshenv";
  programs.tmux = {
    enable = true;
    shell = "${pkgs.zsh}/bin/zsh";
    mouse = true;
    extraConfig = ''
      set -g mouse on
      setw -g mode-keys vi

      # Use Vim keybindings in copy-mode
      bind -Tcopy-mode-vi 'v' send -X begin-selection
      bind -Tcopy-mode-vi 'y' send -X copy-selection-and-cancel
      bind -Tcopy-mode-vi 'V' send -X select-line

      # Pane switching with vim keys
      bind h select-pane -L
      bind j select-pane -D
      bind k select-pane -U
      bind l select-pane -R

      # Resize panes with vim keys + prefix
      bind -r H resize-pane -L 5
      bind -r J resize-pane -D 5
      bind -r K resize-pane -U 5
      bind -r L resize-pane -R 5

      # Split panes Vim-style
      bind | split-window -h
      bind - split-window -v

      # Reload configuration
      bind r source-file ~/.config/tmux/tmux.conf \; display-message "tmux reloaded!"

      # Better status bar
      set -g status-bg black
      set -g status-fg white
      set -g status-interval 5

      # Use 256 colors
      set -g default-terminal "screen-256color"
      set -ga terminal-overrides ",xterm-256color:Tc"
    '';
  };
}
