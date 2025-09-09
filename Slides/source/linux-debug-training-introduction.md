---
marp: true
theme: uncover
size: 16:9
paginate: true
title: Linux Debug Training
footer: "[Linux Debug Training](https://github.com/SpecialistLinuxTraining/linux-debug-training) © 2024-2025 [John O'Sullivan](https://www.linkedin.com/in/john-o-15b17a34/) | [Manas Marawaha](https://www.linkedin.com/in/manasmarwah/) is licensed under [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)"
---
<style>
  section { background-image: url('../../Resources/png/background.png'); background-size: contain; font-family: 'Times New Roman', serif !important; background-color: #fff; text-align: left; top: 20; font-size: 26px;}
  h1 {   font-size: 50px;   color: #09c;   position: absolute; padding: 20px;   top: 0;   left: 0;   right:  0;  text-align: center;}
  h2 {   font-size: 52px;   color: #09c;   position: absolute; padding: 20px;   top: 20;   left: 0;   right:  0;  text-align: center;} 
  h3 {   font-size: 52px;   color: #09c;   position: relative; padding: 0px;   top: 0;   left: 0;   right:  0;  text-align: center;} 
  h5 {   font-size: 20px;   color: #09c;   position: absolute; padding: 10px;   top: 5;  left: 0;   right:  0; } 

footer {
  text-align: center;
  position: absolute;
  padding: 4px;
  bottom: 0;
  left: 0;
  right:  0;
  z-index: 5; 
  color: #09c;
  background: none;
  font-size: 14px;
  text-align: center;
}

section.no-footer footer {
    display: none;
}

ul {
  text-align: left;
  margin-left: 1.5em;
}

li {
    text-align: left;
  }

.left-align {
  text-align: left;
}

.baseline-container {
  display: flex;
  justify-content: center;
  align-items: flex-end;
  height: 100vh;
}

.container {
  display: flex;
}

.column {
  flex: 1;
  text-align: center;
  padding: 1rem;
}


.image-container-baseline {
  margin-bottom: 0rem;
}

.code-block {
  font-size: 16px;
}

.image-container {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100%;
  margin-bottom: 2rem;
}

.logo-container {
  position: absolute;
  top: 43%;
  left: 50%;
  transform: translate(-50%, -50%);
  display: flex;
  justify-content: center;
  align-items: center;
  height: auto;
  width: auto;
}

.large-logo-container {
  position: relative;
  padding: 0px;
  top: 10%;
  left: 50%;
  transform: translate(-50%, -50%);
  display: flex;
  justify-content: center;
  align-items: center;
  height: auto;
  width: auto;
}

.presenter-picture {
  width: 200px;
  height: 200px;
  border-radius: 50%;
  overflow: hidden;
  margin: 0 auto 1rem;
  border: 3px solid #09c;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  }

.presenter-picture img {
  width: 100%;
  height: 100%;
  object-fit: cover;
}

.presenter-name {
  font-family: "Arial", sans-serif;
  font-size: 30px;
  font-weight: bold;
  color: #222;
  margin-top: 1rem;
}

.introduction {
  font-family: "Verdana", sans-serif;
  font-size: 16px;
  color: #444;
  margin-top: 0.5rem;
}
  
.bottom-container {
  position: absolute;
  bottom: 0;
  left: 0;
  width: 100%;
  padding: 10px 20px 30px;
  background-color: transparent;
  font-family: "Verdana", sans-serif;
  font-size: 16px;
  line-height: 1.6;
  text-align: center;
}

.bottom-container span {
  display: inline-block;
}

.two-row-container {
  display: flex;
  flex-direction: column;
  height: 100%;
  margin-top: 6rem; 
  justify-content: left;
}

.row {
  font-size: 20px;
  line-height: 2;
  margin: 0;
  padding: 0;
}

.two-column-container {
  display: flex;
}

.presenter-into-container {
  display: flex;
  justify-content: space-between; 
  align-items: flex-start; 
  height: 100%; 
  padding: 2rem;
}

.code-block-small {
  font-size: 12px;
}

.contact-container {
  display: flex; 
  gap: 60px; 
  justify-content: center;
  align-items: center;
  margin-top: 10px; 
}

.contact-link {
  color: #09c; 
  font-size: 16px;
  text-decoration: none;
  font-weight: bold;
}

.contact-link:hover {
  text-decoration: underline; 
  color: #0056b3; 
}

.course-outline-container {
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  align-items: flex-start;
  padding: 10px;
  margin: 50px 10px 50px 10px;
  height: 200vh; 
  line-height: 1.5;
  background-color: transparent; 
  font-family: "Arial", sans-serif;
  color: #333;
  overflow-y: auto;
  overflow-x: hidden;
  box-sizing: border-box;
}

.course-title {
  font-size: 38px;
  font-weight: bold;
  margin-bottom: 20px;
  color: #0056b3;
  text-align: center;
  width: 100%;
}

.course-title a {
  color: #0056b3;
  text-decoration: none;
  font-weight: bold;
}

.course-title a:hover {
  text-decoration: underline; 
  color: #003f80;
}

.module-list {
  list-style-type: none;
  padding: 0;
  margin: 10;
  width: 100%;
}

.module-item {
  font-size: 30px;
  margin-bottom: 10px;
  display: flex;
  align-items: center;
}

.module-item span {
  font-weight: bold;
  color: #0056b3;
  margin-right: 10px;
}

.module-item p {
  margin: 0;
  font-weight: bold;
  color: #444;
}

</style>


<!-- _class: no-footer -->

<h1> <br>Linux Debug Training </h1>

<div class="logo-container">
<img height="225" src="../../Resources/png/logo_penguin.png" alt="Logo">
</div>

<div class="bottom-container">

<span> **Presenters**: [John O'Sullivan](https://www.linkedin.com/in/john-o-15b17a34/) and [Manas Marawaha](https://www.linkedin.com/in/manasmarwah/) </span>

<span>© Copyright 2024-2025, John O'Sullivan and Manas Marawaha
Licensed under [Creative Commons BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)  (CC BY-SA 4.0)  </span>

<span> **Document source**: https://github.com/SpecialistLinuxTraining/linux-debug-training/tree/main/Slides </span>

<span> **Part-1 Demonstrations**: https://www.udemy.com/course/linux-debug-training-part-1/?referralCode=6534D858AEF9555AB23F </span>

<span> **Part-2 Demonstrations**: https://www.udemy.com/course/linux-debug-training-part-2/?referralCode=1FAF95060F47194A895F </span>
</div>

---
<h1>Presenters Introduction</h1>

<br>
<div class="presenter-into-container">
  <div class="column">
    <div class="presenter-picture">
      <img src="../../Resources/png/JohnOSullivan.jpeg" alt="John O' Sullivan">
    </div>
    <div class="presenter-name">John O'Sullivan</div>
    <div class="introduction">
      <p>John O'Sullivan is a software architect with over 30 years of experience, specializing in the development and optimization of Linux-based embedded systems. He has extensive hands-on expertise in hardware-software integration and product development across a wide range of industries. </p>
    </div>
  <div class="contact-container">
    <a href="mailto:johnos3747@gmail.com" class="contact-link">
      Email
    </a>
    <a href="https://www.linkedin.com/in/john-o-15b17a34/" target="_blank" class="contact-link">
      LinkedIn
    </a>
  </div>
  </div>

  <div class="column">
    <div class="presenter-picture">
      <img src="../../Resources/png/manas-marawaha.JPG" alt="Manas Marawaha">
    </div>
    <div class="presenter-name">Manas Marawaha</div>
    <div class="introduction">
      <p>Manas Marawaha is a Principal Engineer with over 13 years of experience in embedded Linux software development. With a strong understanding of Linux internals, kernel programming, and device drivers, he brings extensive hands-on expertise in product development across audio, video, and networking domains.
</p>
    </div>
  <div class="contact-container">
    <a href="mailto:manas.marwah@gmail.com" class="contact-link">
      Email
    </a>
    <a href="https://www.linkedin.com/in/manasmarwah/" target="_blank" class="contact-link">
      LinkedIn
    </a>
  </div>
</div>

---
# License Information
<span>© Copyright 2024-2025, John O'Sullivan and Manas Marawaha
Licensed under [Creative Commons BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)  (CC BY-SA 4.0) 
https://creativecommons.org/licenses/by-sa/4.0/ </span>

<br>

**You are free to:**

- **Share** - copy and redistribute the material in any medium or format for any purpose, even commercially.

- **Adapt** - remix, transform, and build upon the material for any purpose, even commercially.

- The licensor cannot revoke these freedoms as long as you follow the license terms.

---
# License Information (Cont.)
**Under the following terms:**

- **Attribution** - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

- **ShareAlike** - If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

- **No additional restrictions** - You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
---
<h1>Course information</h1>
<br>
<br>

- Example code demonstrated in the course is available on the [GitHub repository](https://github.com/SpecialistLinuxTraining/linux-debug-training/tree/main/Examples).

- For document source, please refer to this [link](https://github.com/SpecialistLinuxTraining/linux-debug-training/tree/main/Slides).

- Demonstrations in this part of the course use Ubuntu (OS 22.04), Arch Linux ( 2025.08.01) and a Buildroot based installation on the Raspberry-Pi.

- Please review the [instructions](https://github.com/SpecialistLinuxTraining/linux-debug-training/blob/main/References/building_course_examples.md) for building the course examples before proceeding.

- We value your feedback and suggestions! Please share them via email at specialistlinuxtraining@gmail.com.

- For corrections on the training material, feel free to raise [issues](https://github.com/SpecialistLinuxTraining/linux-debug-training/issues) and [pull request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork) in the GitHub repository.

---
# Course Outline
<div class="course-outline-container">
<div class="course-title">
  <a href="https://www.udemy.com/course/linux-debug-training-part-1/?referralCode=6534D858AEF9555AB23F" target="_blank">Linux Debug Training (Part-1)</a>
</div>
  <ul class="module-list">
    <li class="module-item">
      <span>Module 1:</span>
      <p>Linux Operating System Architecture</p>
    </li>
    <li class="module-item">
      <span>Module 2:</span>
      <p>Basic Linux Analysis and Observability Tools</p>
    </li>
    <li class="module-item">
      <span>Module 3:</span>
      <p>Application Debugging</p>
    </li>
    <li class="module-item">
      <span>Module 4:</span>
      <p>Memory Issues in Linux Applications</p>
    </li>
    <div class="course-title"> Linux Debug Training (Part-2)</div>
    <li class="module-item">
      <span>Module 5:</span>
      <p>Tracing in Linux</p>
    </li>
    <li class="module-item">
      <span>Module 6:</span>
      <p>Profiling in Linux</p>
    </li>
    <li class="module-item">
      <span>Module 7:</span>
      <p>Linux Kernel Debugging</p>
    </li>
  </ul>
</div>

---
<h1>Part-1 Synopsis</h1>

- Module 1 began with the Linux OS system itself, discussing the fundamental architecture and structure of the Linux operating system.

- In the second module of this course, we explored the Basic Linux Analysis and Observability Tools. We demonstrated how these foundational tools served as the first level of debugging and how they provided insights into the system's state and behavior.

- The third module of this course looked at how we could dissect application binaries with tools like binutils and how we could employ powerful debugging applications like GDB for more in-depth analysis.

- The fourth module was dedicated to addressing common memory issues in user-space applications, along with exploring related tools such as valgrind and sanitizers which are designed to detect and resolve these issues before the code is deployed.

---
<h1>Part-2 Synopsis</h1>
<br>

- Module five takes a look at tracing in Linux. We cover userspace tools like strace, ltrace, uprobe and perf. We also look at kernel space tools like Kprobe, Perf, ftrace, eBPF, and LTTng.

- In the sixth module of this course, we will explore profiling in Linux looking at tools like massif, heaptrack and memusage to profile memory. We will investigate the use of callgrind, cachegrind and perf-stat for CPU and hardware profiling. We will also look at stacktrace profiling using eBPF profile, gprof and sysprof. And, we will look at how data visualization tools can complement our analysis.

- In the seventh and final module we take a comprehensive look at Kernel debugging, investigating Kernel OPPS, reviewing logging and SysRq, and using tools like KGDB. We will show Kernel recovery using Kexec and Kdump. And we will also examine many of the new tools that have emerged in recent years like: UBSAN, KCSAN and KASAN.

