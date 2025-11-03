#!/bin/bash
# Script to connect your local project to your GitLab private repository

echo "=== Connecting to Your GitLab Private Repository ==="
echo ""

# Check if we need to get the SSH URL
read -p "Enter your GitLab SSH URL (git@gitlab.imt-atlantique.fr:username/simple-unix-shell.git): " ssh_url

if [ -z "$ssh_url" ]; then
    echo "Error: SSH URL is required!"
    exit 1
fi

echo ""
echo "Removing old remote (template repository)..."
git remote remove origin 2>/dev/null || true

echo "Adding your private repository as remote..."
git remote add origin "$ssh_url"

echo ""
echo "Remote configured:"
git remote -v

echo ""
echo "=== Ready to push! ==="
echo ""
echo "Next, run these commands:"
echo "  git add ."
echo "  git commit -m 'Initial commit: Unix shell project'"
echo "  git push -u origin main"
echo ""

