# Quick Start Guide - GitLab & WSL Setup

## ✅ You Have:
- ✅ WSL (Ubuntu 24.04) installed
- ✅ Project cloned to `processes-shell/`
- ✅ Ready to set up GitLab collaboration

## 🚀 Quick Setup Steps:

### Step 1: Open WSL and Navigate to Project

In your terminal (PowerShell or Command Prompt), run:
```powershell
wsl
```

Then in WSL:
```bash
cd "/mnt/c/Users/lenovo/OS project/processes-shell"
```

### Step 2: Run Setup Script

```bash
chmod +x setup-wsl.sh
./setup-wsl.sh
```

This will:
- Check/install git
- Generate SSH key (if needed)
- Initialize git repository

### Step 3: Create GitLab Private Repository

1. **Go to:** https://gitlab.imt-atlantique.fr
2. **Click:** "New Project" → "Create blank project"
3. **Fill in:**
   - Project name: `simple-unix-shell` (or your choice)
   - Visibility: **Private**
4. **Copy the SSH URL** shown (looks like: `git@gitlab.imt-atlantique.fr:username/project.git`)

### Step 4: Add SSH Key to GitLab

If the setup script generated a key, it showed it. Copy it and:
1. Go to: https://gitlab.imt-atlantique.fr/-/profile/keys
2. Click "Add new key"
3. Paste your public key
4. Save

### Step 5: Connect Your Local Project to GitLab

In WSL (inside your project directory):
```bash
# Add GitLab as remote (replace with YOUR actual URL from Step 3)
git remote add origin git@gitlab.imt-atlantique.fr:YOUR_USERNAME/YOUR_PROJECT_NAME.git

# Verify it was added
git remote -v
```

### Step 6: Push Your Code

```bash
# Stage all files
git add .

# Make first commit
git commit -m "Initial commit: Unix shell project setup"

# Push to GitLab
git push -u origin main
```

### Step 7: Add Your Group Member

1. In GitLab, go to: **Settings** → **Members**
2. Click **"Invite members"**
3. Enter their GitLab username/email
4. Role: **Developer**
5. Click **"Add to project"**

### Step 8: Test Compilation

```bash
# Compile your shell
make

# Run tests (once you have implementation)
./test-wish.sh
```

## 📋 Daily Workflow

### When Starting Work:
```bash
wsl
cd "/mnt/c/Users/lenovo/OS project/processes-shell"
git pull origin main  # Get latest changes from your partner
```

### After Making Changes:
```bash
git add .
git commit -m "Description of what you changed"
git push origin main
```

## 📚 Need More Details?

- See `GITLAB_SETUP.md` for detailed GitLab setup
- See `SETUP.md` for implementation guide
- See `README.md` for full project requirements

## 🔧 Common Issues

**"Permission denied" when pushing:**
- Make sure SSH key is added to GitLab
- Test: `ssh -T git@gitlab.imt-atlantique.fr`

**"Repository not found":**
- Check the remote URL: `git remote -v`
- Make sure you have access to the repository
- Verify your group member added you as collaborator

**"No space left":**
- The project is small, this shouldn't happen
- But if it does, clean WSL: `wsl --shutdown` then restart

---

**Ready to start coding!** 🎉

