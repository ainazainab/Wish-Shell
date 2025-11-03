#!/bin/bash
# Script to set up the project in WSL for GitLab collaboration

echo "=== Unix Shell Project Setup ==="
echo ""

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Installing git..."
    sudo apt update
    sudo apt install -y git
fi

# Check if SSH key exists
if [ ! -f ~/.ssh/id_ed25519.pub ]; then
    echo "=== Generating SSH Key ==="
    read -p "Enter your email for SSH key: " email
    ssh-keygen -t ed25519 -C "$email" -f ~/.ssh/id_ed25519 -N ""
    echo ""
    echo "Your SSH public key:"
    cat ~/.ssh/id_ed25519.pub
    echo ""
    echo "Please copy this key and add it to GitLab:"
    echo "https://gitlab.imt-atlantique.fr/-/profile/keys"
    echo ""
    read -p "Press Enter after you've added the key to GitLab..."
fi

# Initialize git if not already done
if [ ! -d .git ]; then
    echo "=== Initializing Git Repository ==="
    git init
    git branch -M main
    echo "Git repository initialized!"
else
    echo "Git repository already exists."
fi

# Check git remote
echo ""
echo "=== Checking Git Remote ==="
if git remote -v | grep -q origin; then
    echo "Git remote 'origin' is already configured:"
    git remote -v
else
    echo "No git remote configured yet."
    echo ""
    echo "To add your GitLab repository, run:"
    echo "  git remote add origin git@gitlab.imt-atlantique.fr:YOUR_USERNAME/YOUR_PROJECT_NAME.git"
    echo ""
    echo "Or if you prefer HTTPS:"
    echo "  git remote add origin https://gitlab.imt-atlantique.fr/YOUR_USERNAME/YOUR_PROJECT_NAME.git"
fi

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Next steps:"
echo "1. Create a private repository on GitLab (see GITLAB_SETUP.md)"
echo "2. Add your group member as a collaborator"
echo "3. Add the remote repository (command shown above)"
echo "4. Commit and push your code:"
echo "   git add ."
echo "   git commit -m 'Initial commit'"
echo "   git push -u origin main"
echo ""

