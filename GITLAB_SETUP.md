# GitLab Setup Guide for Collaboration

## Step 1: Create Private Repository on GitLab

1. **Log in to GitLab IMT Atlantique:**
   - Go to: https://gitlab.imt-atlantique.fr/users/sign_in
   - Sign in with your IMT credentials

2. **Create a New Project:**
   - Click **"New Project"** or **"Create project"**
   - Choose **"Create blank project"**
   - **Project name:** `simple-unix-shell` (or any name you prefer)
   - **Visibility level:** Select **"Private"** 
   - Click **"Create project"**

3. **Copy the Repository URL:**
   - After creating, you'll see the repository page
   - Copy the **SSH URL** (looks like: `git@gitlab.imt-atlantique.fr:username/simple-unix-shell.git`)
   - Or copy the **HTTPS URL** (looks like: `https://gitlab.imt-atlantique.fr/username/simple-unix-shell.git`)

## Step 2: Add Your Group Member as Collaborator

1. In your GitLab project, go to **"Settings"** → **"Members"**
2. Click **"Invite members"**
3. Enter your group member's GitLab username or email
4. Select role: **"Developer"** (or "Maintainer" if you want them to have more permissions)
5. Click **"Add to project"**
6. Your group member will receive an invitation email

## Step 3: Set Up SSH Key (Recommended for WSL)

SSH keys allow you to push/pull without entering credentials every time.

### In WSL:

1. **Generate SSH Key (if you don't have one):**
   ```bash
   ssh-keygen -t ed25519 -C "your_email@imt-atlantique.fr"
   ```
   - Press Enter to accept default location
   - Optionally set a passphrase

2. **Display your public key:**
   ```bash
   cat ~/.ssh/id_ed25519.pub
   ```
   - Copy the entire output (starts with `ssh-ed25519 ...`)

3. **Add SSH Key to GitLab:**
   - Go to: https://gitlab.imt-atlantique.fr/-/profile/keys
   - Click **"Add new key"**
   - Paste your public key
   - Add a title: "WSL Key"
   - Click **"Add key"**

4. **Test SSH Connection:**
   ```bash
   ssh -T git@gitlab.imt-atlantique.fr
   ```
   - Should see: "Welcome to GitLab, @username!"

## Step 4: Push Your Code to GitLab

### Option A: Using SSH (Recommended)

1. **In WSL, navigate to your project:**
   ```bash
   cd ~/path/to/processes-shell
   ```

2. **Initialize git (if not already done):**
   ```bash
   git init
   ```

3. **Add files:**
   ```bash
   git add .
   git commit -m "Initial commit: Unix shell project setup"
   ```

4. **Add remote repository:**
   ```bash
   git remote add origin git@gitlab.imt-atlantique.fr:YOUR_USERNAME/YOUR_PROJECT_NAME.git
   ```
   (Replace with your actual SSH URL from Step 1)

5. **Push to GitLab:**
   ```bash
   git branch -M main  # Rename branch to main
   git push -u origin main
   ```

### Option B: Using HTTPS

If you prefer HTTPS (you'll need to enter credentials):

1. Use the HTTPS URL instead:
   ```bash
   git remote add origin https://gitlab.imt-atlantique.fr/YOUR_USERNAME/YOUR_PROJECT_NAME.git
   ```

2. Push:
   ```bash
   git push -u origin main
   ```

## Step 5: Collaborating with Your Group Member

### For Both of You:

1. **Clone the repository (if your group member needs to clone):**
   ```bash
   git clone git@gitlab.imt-atlantique.fr:YOUR_USERNAME/YOUR_PROJECT_NAME.git
   cd YOUR_PROJECT_NAME
   ```

2. **Daily Workflow:**
   ```bash
   # Before starting work, get latest changes
   git pull origin main
   
   # After making changes
   git add .
   git commit -m "Description of changes"
   git push origin main
   ```

3. **If there are conflicts:**
   ```bash
   git pull origin main
   # Resolve conflicts, then:
   git add .
   git commit -m "Resolved conflicts"
   git push origin main
   ```

## Troubleshooting

### SSH Connection Issues:
- Make sure your public key is added to GitLab
- Check: `ssh -T git@gitlab.imt-atlantique.fr`

### Permission Denied:
- Make sure your group member has been added as a collaborator
- Check their SSH key is set up correctly

### HTTPS Authentication:
- You may need to create a Personal Access Token
- Go to: Settings → Access Tokens
- Create token with `write_repository` scope

## Next Steps

Once set up:
1. Continue development in WSL
2. Test your code: `make` and `./test-wish.sh`
3. Commit and push regularly
4. Communicate with your group member about who's working on what

Good luck with your project!

