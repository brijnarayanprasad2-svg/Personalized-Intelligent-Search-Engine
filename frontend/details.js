/* =========================================================
   PISE — TOPIC DETAILS
   ========================================================= */

const topics = {

    "computer_science": {
        title: "Computer Science",
        category: "COMPUTER SCIENCE",
        description:
            "Computer Science is the study of computation, algorithms, data, software and the systems used to solve computational problems.",

        tags: [
            "Computer Science",
            "Algorithms",
            "Programming"
        ],

        related: [
            ["algorithms", "Algorithms"],
            ["data_structures", "Data Structures"],
            ["cpp_programming", "C++ Programming"]
        ],

        content: `
            <h2>About Computer Science</h2>

            <p>
                Computer Science is a field that deals with computation,
                algorithms, programming, data and computer-based systems.
                It provides the foundation for many areas of modern
                technology.
            </p>

            <h3>What does Computer Science cover?</h3>

            <p>
                The subject includes programming languages, data structures,
                algorithms, databases, operating systems, computer networks,
                artificial intelligence and software development.
            </p>

            <ul>
                <li>Programming and problem solving</li>
                <li>Data structures and algorithms</li>
                <li>Database management</li>
                <li>Computer networks</li>
                <li>Artificial intelligence</li>
                <li>Software development</li>
            </ul>

            <h3>Role in PISE</h3>

            <p>
                Computer Science is one of the main topics indexed by PISE.
                The search engine can retrieve this page using keywords
                related to programming, algorithms and computer systems.
            </p>
        `
    },


    "data_structures": {
        title: "Data Structures",
        category: "DATA STRUCTURES",
        description:
            "Data structures provide organized ways to store, manage and access data efficiently.",

        tags: [
            "Data Structures",
            "DSA",
            "C++"
        ],

        related: [
            ["algorithms", "Algorithms"],
            ["cpp_programming", "C++ Programming"],
            ["database_management", "Database Management"]
        ],

        content: `
            <h2>About Data Structures</h2>

            <p>
                A data structure is a way of organizing and storing data so
                that operations such as searching, insertion, deletion and
                traversal can be performed efficiently.
            </p>

            <h3>Common Data Structures</h3>

            <ul>
                <li>Arrays</li>
                <li>Linked Lists</li>
                <li>Stacks</li>
                <li>Queues</li>
                <li>Trees</li>
                <li>Graphs</li>
                <li>Hash Tables</li>
                <li>Heaps</li>
            </ul>

            <h3>Why are they important?</h3>

            <p>
                Choosing the right data structure can significantly improve
                the efficiency of a program. Search engines, databases and
                operating systems all depend on suitable data organization.
            </p>

            <h3>Role in PISE</h3>

            <p>
                Data structures are central to our search engine. The project
                uses a Trie for vocabulary and autocomplete operations,
                hash-based storage for search information and a priority
                queue for ranking suggestions.
            </p>
        `
    },


    "algorithms": {
        title: "Algorithms",
        category: "ALGORITHMS",
        description:
            "Algorithms are step-by-step procedures used to solve computational problems efficiently.",

        tags: [
            "Algorithms",
            "Problem Solving",
            "Optimization"
        ],

        related: [
            ["data_structures", "Data Structures"],
            ["computer_science", "Computer Science"],
            ["cpp_programming", "C++ Programming"]
        ],

        content: `
            <h2>About Algorithms</h2>

            <p>
                An algorithm is a sequence of well-defined steps used to
                solve a problem or perform a particular computation.
            </p>

            <h3>Important Algorithm Concepts</h3>

            <ul>
                <li>Searching</li>
                <li>Sorting</li>
                <li>Recursion</li>
                <li>Graph traversal</li>
                <li>Dynamic programming</li>
                <li>Greedy approaches</li>
            </ul>

            <h3>Efficiency</h3>

            <p>
                Algorithm efficiency is commonly discussed using time and
                space complexity. Big-O notation helps describe how the
                required resources change as the input size increases.
            </p>

            <h3>Role in PISE</h3>

            <p>
                PISE uses algorithmic processing for query normalization,
                spelling correction, indexing and ranking. These operations
                work together to produce useful search results.
            </p>
        `
    },


    "cpp_programming": {
        title: "C++ Programming",
        category: "PROGRAMMING",
        description:
            "C++ is a general-purpose programming language that supports procedural, object-oriented and generic programming.",

        tags: [
            "C++",
            "Programming",
            "OOP"
        ],

        related: [
            ["data_structures", "Data Structures"],
            ["algorithms", "Algorithms"],
            ["object_oriented_programming", "Object-Oriented Programming"]
        ],

        content: `
            <h2>About C++</h2>

            <p>
                C++ is a powerful programming language commonly used for
                system software, applications, competitive programming and
                performance-oriented development.
            </p>

            <h3>Important Features</h3>

            <ul>
                <li>Procedural programming</li>
                <li>Object-oriented programming</li>
                <li>Templates and generic programming</li>
                <li>Standard Template Library</li>
                <li>Manual and automatic resource management</li>
            </ul>

            <h3>Role in PISE</h3>

            <p>
                The core search engine of PISE is implemented in C++.
                Classes are used to organize the different components,
                while standard data structures support search, indexing
                and ranking operations.
            </p>
        `
    },


    "object_oriented_programming": {
        title: "Object-Oriented Programming",
        category: "PROGRAMMING",
        description:
            "Object-Oriented Programming organizes software around objects and classes to make large programs easier to manage.",

        tags: [
            "OOP",
            "Classes",
            "C++"
        ],

        related: [
            ["cpp_programming", "C++ Programming"],
            ["data_structures", "Data Structures"],
            ["computer_science", "Computer Science"]
        ],

        content: `
            <h2>About Object-Oriented Programming</h2>

            <p>
                Object-Oriented Programming, commonly called OOP, is a
                programming approach in which software is organized using
                classes and objects.
            </p>

            <h3>Main Concepts</h3>

            <ul>
                <li>Classes and objects</li>
                <li>Encapsulation</li>
                <li>Abstraction</li>
                <li>Inheritance</li>
                <li>Polymorphism</li>
            </ul>

            <h3>Role in PISE</h3>

            <p>
                The project follows an object-oriented structure. Components
                such as Trie, WebCrawler, WebPage, SearchIndex,
                RankingEngine and SearchHistory are organized as separate
                classes with specific responsibilities.
            </p>

            <p>
                This modular structure makes the project easier to test,
                maintain and extend.
            </p>
        `
    },


    "data_science": {
        title: "Data Science",
        category: "DATA & ANALYTICS",
        description:
            "Data Science combines data analysis, statistics and computational methods to discover useful patterns from data.",

        tags: [
            "Data Science",
            "Analytics",
            "Data"
        ],

        related: [
            ["machine_learning", "Machine Learning"],
            ["artificial_intelligence", "Artificial Intelligence"],
            ["database_management", "Database Management"]
        ],

        content: `
            <h2>About Data Science</h2>

            <p>
                Data Science is concerned with collecting, processing,
                analyzing and interpreting data to obtain useful information
                and support decision making.
            </p>

            <h3>Common Activities</h3>

            <ul>
                <li>Data collection</li>
                <li>Data cleaning</li>
                <li>Data analysis</li>
                <li>Visualization</li>
                <li>Pattern identification</li>
                <li>Statistical analysis</li>
            </ul>

            <h3>Data Science and Machine Learning</h3>

            <p>
                Machine Learning is often used as one part of a larger data
                science workflow. Data preparation and analysis are important
                before a model can produce useful results.
            </p>

            <h3>Role in PISE</h3>

            <p>
                Data Science is included in the local indexed corpus of PISE.
                Users can search for this topic using normal keywords or
                supported query variations.
            </p>
        `
    },


    "machine_learning": {
        title: "Machine Learning",
        category: "ARTIFICIAL INTELLIGENCE",
        description:
            "Machine Learning enables computer systems to learn patterns from data and use those patterns to make predictions or decisions.",

        tags: [
            "Machine Learning",
            "AI",
            "Data"
        ],

        related: [
            ["artificial_intelligence", "Artificial Intelligence"],
            ["data_science", "Data Science"],
            ["computer_science", "Computer Science"]
        ],

        content: `
            <h2>About Machine Learning</h2>

            <p>
                Machine Learning is a field of computing in which algorithms
                learn patterns from data instead of relying only on explicitly
                written rules.
            </p>

            <h3>Common Types</h3>

            <ul>
                <li>Supervised Learning</li>
                <li>Unsupervised Learning</li>
                <li>Reinforcement Learning</li>
            </ul>

            <h3>Applications</h3>

            <p>
                Machine Learning is used in recommendation systems,
                classification, prediction, image processing, natural
                language processing and many other applications.
            </p>

            <h3>Role in PISE</h3>

            <p>
                Machine Learning is currently part of the indexed topic
                collection. Advanced machine-learning based semantic search
                can be considered as a future extension of the project.
            </p>
        `
    },


    "artificial_intelligence": {
        title: "Artificial Intelligence",
        category: "ARTIFICIAL INTELLIGENCE",
        description:
            "Artificial Intelligence focuses on building systems that can perform tasks normally associated with intelligent behaviour.",

        tags: [
            "Artificial Intelligence",
            "AI",
            "Intelligent Systems"
        ],

        related: [
            ["machine_learning", "Machine Learning"],
            ["data_science", "Data Science"],
            ["computer_science", "Computer Science"]
        ],

        content: `
            <h2>About Artificial Intelligence</h2>

            <p>
                Artificial Intelligence is a field of computer science that
                focuses on developing systems capable of performing tasks
                such as reasoning, learning, prediction and decision making.
            </p>

            <h3>Major Areas</h3>

            <ul>
                <li>Machine Learning</li>
                <li>Natural Language Processing</li>
                <li>Computer Vision</li>
                <li>Knowledge Representation</li>
                <li>Intelligent Agents</li>
            </ul>

            <h3>Role in PISE</h3>

            <p>
                The current project focuses on data structures, search
                algorithms and personalization. AI-based semantic retrieval
                is considered a possible future extension.
            </p>
        `
    },


    "internet_protocols": {
        title: "Internet and Protocols",
        category: "COMPUTER NETWORKS",
        description:
            "Internet protocols define the rules that allow devices and applications to communicate across networks.",

        tags: [
            "Internet",
            "Protocols",
            "Networking"
        ],

        related: [
            ["computer_science", "Computer Science"],
            ["database_management", "Database Management"]
        ],

        content: `
            <h2>About Internet Protocols</h2>

            <p>
                Internet communication depends on a collection of protocols
                that define how data is addressed, transferred and received
                between systems.
            </p>

            <h3>Common Protocols</h3>

            <ul>
                <li>HTTP and HTTPS</li>
                <li>TCP</li>
                <li>IP</li>
                <li>DNS</li>
                <li>FTP</li>
            </ul>

            <h3>HTTP and Web Search</h3>

            <p>
                Web browsers and web servers commonly communicate using HTTP
                or HTTPS. These protocols allow a client to request resources
                and receive responses from a server.
            </p>

            <h3>Role in PISE</h3>

            <p>
                The crawler component of PISE uses HTTP communication when
                accessing crawlable web resources, while the local API uses
                HTTP to connect the browser frontend with the C++ search core.
            </p>
        `
    },


    "database_management": {
        title: "Database Management",
        category: "DATA MANAGEMENT",
        description:
            "Database Management deals with storing, organizing, retrieving and maintaining structured information.",

        tags: [
            "Database",
            "Data Management",
            "Storage"
        ],

        related: [
            ["data_science", "Data Science"],
            ["data_structures", "Data Structures"],
            ["computer_science", "Computer Science"]
        ],

        content: `
            <h2>About Database Management</h2>

            <p>
                Database Management provides methods for storing and
                retrieving structured data efficiently. Database systems are
                widely used in applications that need reliable and organized
                information storage.
            </p>

            <h3>Important Concepts</h3>

            <ul>
                <li>Tables and records</li>
                <li>Primary keys</li>
                <li>Relationships</li>
                <li>Queries</li>
                <li>Indexes</li>
                <li>Transactions</li>
            </ul>

            <h3>Role in PISE</h3>

            <p>
                Database Management is currently represented as one of the
                searchable topics in the local corpus. Database-oriented
                storage can also be considered for future large-scale
                versions of the search engine.
            </p>
        `
    }
};


/* =========================================================
   SVG TOPIC ILLUSTRATION
   ========================================================= */

function createTopicVisual(topicKey) {

    const visuals = {

        "data_science": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg"
                 aria-label="Data science illustration">

                <circle cx="160" cy="95" r="58"
                        fill="none"
                        stroke="#9da8ff"
                        stroke-width="2"
                        opacity=".35"/>

                <circle cx="160" cy="95" r="10"
                        fill="#b9c0ff"/>

                <circle cx="75" cy="55" r="7"
                        fill="#9da8ff"/>

                <circle cx="250" cy="55" r="7"
                        fill="#9da8ff"/>

                <circle cx="80" cy="145" r="7"
                        fill="#9da8ff"/>

                <circle cx="245" cy="145" r="7"
                        fill="#9da8ff"/>

                <line x1="82" y1="57"
                      x2="150" y2="90"
                      stroke="#7784ed"
                      opacity=".5"/>

                <line x1="243" y1="57"
                      x2="170" y2="90"
                      stroke="#7784ed"
                      opacity=".5"/>

                <line x1="85" y1="142"
                      x2="151" y2="100"
                      stroke="#7784ed"
                      opacity=".5"/>

                <line x1="238" y1="142"
                      x2="170" y2="100"
                      stroke="#7784ed"
                      opacity=".5"/>

                <path d="M95 120
                         C125 105 135 125 155 105
                         S190 70 225 85"
                      fill="none"
                      stroke="#b9c0ff"
                      stroke-width="4"
                      stroke-linecap="round"/>
            </svg>
        `,

        "data_structures": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <rect x="135" y="20"
                      width="50"
                      height="38"
                      rx="8"
                      fill="#9da8ff"
                      opacity=".9"/>

                <rect x="65" y="85"
                      width="50"
                      height="38"
                      rx="8"
                      fill="#737fea"
                      opacity=".9"/>

                <rect x="205" y="85"
                      width="50"
                      height="38"
                      rx="8"
                      fill="#737fea"
                      opacity=".9"/>

                <rect x="25" y="145"
                      width="45"
                      height="30"
                      rx="7"
                      fill="#4f5bc0"/>

                <rect x="90" y="145"
                      width="45"
                      height="30"
                      rx="7"
                      fill="#4f5bc0"/>

                <rect x="185" y="145"
                      width="45"
                      height="30"
                      rx="7"
                      fill="#4f5bc0"/>

                <rect x="250" y="145"
                      width="45"
                      height="30"
                      rx="7"
                      fill="#4f5bc0"/>

                <line x1="160" y1="58"
                      x2="90" y2="85"
                      stroke="#aab3ff"
                      stroke-width="3"/>

                <line x1="160" y1="58"
                      x2="230" y2="85"
                      stroke="#aab3ff"
                      stroke-width="3"/>

                <line x1="90" y1="123"
                      x2="47" y2="145"
                      stroke="#7581e8"
                      stroke-width="3"/>

                <line x1="90" y1="123"
                      x2="112" y2="145"
                      stroke="#7581e8"
                      stroke-width="3"/>

                <line x1="230" y1="123"
                      x2="207" y2="145"
                      stroke="#7581e8"
                      stroke-width="3"/>

                <line x1="230" y1="123"
                      x2="272" y2="145"
                      stroke="#7581e8"
                      stroke-width="3"/>
            </svg>
        `,

        "algorithms": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <circle cx="55" cy="95"
                        r="22"
                        fill="#5967d7"/>

                <circle cx="160" cy="55"
                        r="22"
                        fill="#9da8ff"/>

                <circle cx="265" cy="95"
                        r="22"
                        fill="#5967d7"/>

                <circle cx="160" cy="140"
                        r="22"
                        fill="#737fea"/>

                <path d="M76 88 L139 62"
                      stroke="#b9c0ff"
                      stroke-width="4"/>

                <path d="M181 62 L244 88"
                      stroke="#b9c0ff"
                      stroke-width="4"/>

                <path d="M244 105 L181 133"
                      stroke="#b9c0ff"
                      stroke-width="4"/>

                <path d="M139 133 L76 105"
                      stroke="#b9c0ff"
                      stroke-width="4"/>

                <path d="M160 77 L160 118"
                      stroke="#b9c0ff"
                      stroke-width="4"
                      stroke-dasharray="7 7"/>
            </svg>
        `,

        "machine_learning": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <rect x="95"
                      y="35"
                      width="130"
                      height="120"
                      rx="22"
                      fill="none"
                      stroke="#9da8ff"
                      stroke-width="3"/>

                <circle cx="130" cy="75"
                        r="10"
                        fill="#b9c0ff"/>

                <circle cx="190" cy="75"
                        r="10"
                        fill="#b9c0ff"/>

                <circle cx="160" cy="110"
                        r="12"
                        fill="#737fea"/>

                <path d="M130 85
                         C130 105 145 110 160 110
                         C175 110 190 105 190 85"
                      fill="none"
                      stroke="#9da8ff"
                      stroke-width="3"/>

                <line x1="80" y1="55"
                      x2="95" y2="65"
                      stroke="#6875df"
                      stroke-width="4"/>

                <line x1="80" y1="105"
                      x2="95" y2="105"
                      stroke="#6875df"
                      stroke-width="4"/>

                <line x1="225" y1="65"
                      x2="240" y2="55"
                      stroke="#6875df"
                      stroke-width="4"/>

                <line x1="225" y1="105"
                      x2="240" y2="105"
                      stroke="#6875df"
                      stroke-width="4"/>
            </svg>
        `,

        "artificial_intelligence": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <rect x="100"
                      y="35"
                      width="120"
                      height="120"
                      rx="25"
                      fill="none"
                      stroke="#9da8ff"
                      stroke-width="4"/>

                <circle cx="135" cy="75"
                        r="9"
                        fill="#b9c0ff"/>

                <circle cx="185" cy="75"
                        r="9"
                        fill="#b9c0ff"/>

                <circle cx="160" cy="112"
                        r="10"
                        fill="#737fea"/>

                <path d="M130 125
                         Q160 145 190 125"
                      fill="none"
                      stroke="#b9c0ff"
                      stroke-width="3"/>

                <path d="M75 50
                         L100 62
                         M75 95
                         L100 95
                         M245 50
                         L220 62
                         M245 95
                         L220 95"
                      stroke="#6d79df"
                      stroke-width="4"/>
            </svg>
        `,

        "internet_protocols": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <circle cx="160"
                        cy="95"
                        r="55"
                        fill="none"
                        stroke="#9da8ff"
                        stroke-width="3"/>

                <ellipse cx="160"
                         cy="95"
                         rx="25"
                         ry="55"
                         fill="none"
                         stroke="#737fea"
                         stroke-width="3"/>

                <line x1="105"
                      y1="95"
                      x2="215"
                      y2="95"
                      stroke="#b9c0ff"
                      stroke-width="3"/>

                <circle cx="70"
                        cy="95"
                        r="13"
                        fill="#737fea"/>

                <circle cx="250"
                        cy="95"
                        r="13"
                        fill="#737fea"/>

                <line x1="83"
                      y1="95"
                      x2="105"
                      y2="95"
                      stroke="#b9c0ff"
                      stroke-width="4"/>

                <line x1="215"
                      y1="95"
                      x2="237"
                      y2="95"
                      stroke="#b9c0ff"
                      stroke-width="4"/>
            </svg>
        `,

        "database_management": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <ellipse cx="160"
                         cy="45"
                         rx="65"
                         ry="20"
                         fill="#9da8ff"/>

                <path d="M95 45
                         V135
                         C95 157 225 157 225 135
                         V45"
                      fill="none"
                      stroke="#737fea"
                      stroke-width="4"/>

                <ellipse cx="160"
                         cy="90"
                         rx="65"
                         ry="20"
                         fill="none"
                         stroke="#737fea"
                         stroke-width="3"/>

                <ellipse cx="160"
                         cy="135"
                         rx="65"
                         ry="20"
                         fill="none"
                         stroke="#737fea"
                         stroke-width="3"/>
            </svg>
        `,

        "cpp_programming": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <rect x="65"
                      y="45"
                      width="190"
                      height="105"
                      rx="18"
                      fill="#0c1120"
                      stroke="#737fea"
                      stroke-width="3"/>

                <text x="160"
                      y="108"
                      text-anchor="middle"
                      fill="#b9c0ff"
                      font-size="48"
                      font-weight="800"
                      font-family="Arial">
                    C++
                </text>
            </svg>
        `,

        "object_oriented_programming": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <circle cx="160"
                        cy="95"
                        r="38"
                        fill="#737fea"/>

                <circle cx="75"
                        cy="55"
                        r="22"
                        fill="#9da8ff"/>

                <circle cx="245"
                        cy="55"
                        r="22"
                        fill="#9da8ff"/>

                <circle cx="75"
                        cy="140"
                        r="22"
                        fill="#5967d7"/>

                <circle cx="245"
                        cy="140"
                        r="22"
                        fill="#5967d7"/>

                <line x1="94"
                      y1="61"
                      x2="128"
                      y2="82"
                      stroke="#b9c0ff"
                      stroke-width="3"/>

                <line x1="226"
                      y1="61"
                      x2="192"
                      y2="82"
                      stroke="#b9c0ff"
                      stroke-width="3"/>

                <line x1="94"
                      y1="134"
                      x2="128"
                      y2="108"
                      stroke="#b9c0ff"
                      stroke-width="3"/>

                <line x1="226"
                      y1="134"
                      x2="192"
                      y2="108"
                      stroke="#b9c0ff"
                      stroke-width="3"/>
            </svg>
        `,

        "computer_science": `
            <svg viewBox="0 0 320 190"
                 xmlns="http://www.w3.org/2000/svg">

                <rect x="75"
                      y="35"
                      width="170"
                      height="105"
                      rx="15"
                      fill="#0d1323"
                      stroke="#9da8ff"
                      stroke-width="4"/>

                <rect x="100"
                      y="58"
                      width="120"
                      height="60"
                      rx="8"
                      fill="#737fea"
                      opacity=".55"/>

                <line x1="115"
                      y1="155"
                      x2="205"
                      y2="155"
                      stroke="#9da8ff"
                      stroke-width="7"
                      stroke-linecap="round"/>
            </svg>
        `
    };

    return visuals[topicKey] || visuals["computer_science"];
}


/* =========================================================
   GET TOPIC FROM URL
   ========================================================= */

function getTopicKey() {

    const params = new URLSearchParams(
        window.location.search
    );

    return params.get("page") || "computer_science";
}


/* =========================================================
   RENDER HERO
   ========================================================= */

function renderHero(topicKey, topic) {

    const hero = document.getElementById("detailHero");

    if (!hero) {
        return;
    }

    hero.innerHTML = `
        <div class="detail-kicker">
            ${topic.category}
        </div>

        <h1 class="detail-title">
            ${topic.title}
        </h1>

        <p class="detail-description">
            ${topic.description}
        </p>

        <div class="detail-meta">

            ${topic.tags.map(tag => `
                <span class="detail-tag">
                    ${tag}
                </span>
            `).join("")}

        </div>
    `;
}


/* =========================================================
   RENDER ARTICLE
   ========================================================= */

function renderArticle(topicKey, topic) {

    const article = document.getElementById(
        "articleContent"
    );

    if (!article) {
        return;
    }

    article.innerHTML = `

        <div class="topic-visual">
            ${createTopicVisual(topicKey)}
        </div>

        ${topic.content}

    `;
}


/* =========================================================
   RENDER RELATED TOPICS
   ========================================================= */

function renderRelated(topic) {

    const container = document.getElementById(
        "relatedTopics"
    );

    if (!container) {
        return;
    }

    container.innerHTML = `
        <div class="side-kicker">
            RELATED TOPICS
        </div>

        ${topic.related.map(item => `
            <a
                class="related-link"
                href="./details.html?page=${encodeURIComponent(item[0])}"
            >
                <span>${item[1]}</span>
                <span class="related-arrow">→</span>
            </a>
        `).join("")}
    `;
}


/* =========================================================
   PAGE TITLE
   ========================================================= */

function updatePageTitle(topic) {

    document.title =
        `PISE | ${topic.title}`;
}


/* =========================================================
   INITIALIZE
   ========================================================= */

function initializeDetailsPage() {

    const topicKey = getTopicKey();

    const topic = topics[topicKey];

    /*
     * If the requested page does not exist,
     * show Computer Science as a safe fallback.
     */
    if (!topic) {

        const fallback =
            topics["computer_science"];

        renderHero(
            "computer_science",
            fallback
        );

        renderArticle(
            "computer_science",
            fallback
        );

        renderRelated(fallback);

        updatePageTitle(fallback);

        return;
    }


    renderHero(
        topicKey,
        topic
    );

    renderArticle(
        topicKey,
        topic
    );

    renderRelated(topic);

    updatePageTitle(topic);
}


/* =========================================================
   START
   ========================================================= */

document.addEventListener(
    "DOMContentLoaded",
    initializeDetailsPage
);