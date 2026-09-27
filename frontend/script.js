const searchInput = document.getElementById("searchInput");
const searchButton = document.getElementById("searchButton");
const clearButton = document.getElementById("clearButton");
const suggestion = document.getElementById("suggestion");
const resultsSection = document.getElementById("resultsSection");
const resultsContainer = document.getElementById("resultsContainer");
const resultsTitle = document.getElementById("resultsTitle");
const resultCount = document.getElementById("resultCount");


// ==========================================
// DEMO SEARCH DATA
// ==========================================

const demoResults = {

    internet: [
        {
            title: "Internet Assigned Numbers Authority",
            url: "https://iana.org",
            snippet:
                "The global coordination of the DNS root, IP addressing, and other Internet protocol resources is performed by the Internet Assigned Numbers Authority.",
            relevance: 10,
            personalization: 7,
            finalScore: 17
        },

        {
            title: "Number Resources",
            url: "https://iana.org/numbers",
            snippet:
                "We are responsible for global coordination of the Internet Protocol addressing systems and the autonomous system numbers used for routing Internet traffic.",
            relevance: 8,
            personalization: 5,
            finalScore: 13
        }
    ],

    dns: [
        {
            title: "Domain Name Services",
            url: "https://iana.org/domains",
            snippet:
                "We operate and maintain a number of key aspects of the DNS, including the root zone and the .int and .arpa domains.",
            relevance: 3,
            personalization: 5,
            finalScore: 8
        },

        {
            title: "Internet Assigned Numbers Authority",
            url: "https://iana.org",
            snippet:
                "The global coordination of the DNS root, IP addressing and other Internet protocol resources is performed by the Internet Assigned Numbers Authority.",
            relevance: 2,
            personalization: 5,
            finalScore: 7
        }
    ],

    protocol: [
        {
            title: "Protocol Registries",
            url: "https://iana.org/protocols",
            snippet:
                "Protocol parameter registries represent the authoritative record of many of the codes and numbers contained in a variety of Internet protocols.",
            relevance: 10,
            personalization: 5,
            finalScore: 15
        },

        {
            title: "Number Resources",
            url: "https://iana.org/numbers",
            snippet:
                "The procedures, policies and technical specifications describe how Internet number resources are administered.",
            relevance: 4,
            personalization: 5,
            finalScore: 9
        }
    ]
};


// ==========================================
// DISPLAY RESULTS
// ==========================================

function displayResults(query, results) {

    resultsContainer.innerHTML = "";

    resultsTitle.textContent =
        `Results for "${query}"`;

    resultCount.textContent =
        `${results.length} result${results.length === 1 ? "" : "s"}`;

    results.forEach((result, index) => {

        const card =
            document.createElement("article");

        card.className = "result-card";

        card.innerHTML = `
            <div class="result-title">
                ${index + 1}. ${result.title}
            </div>

            <div class="result-url">
                ${result.url}
            </div>

            <div class="result-snippet">
                ${result.snippet}
            </div>

            <div class="result-meta">

                <span class="score">
                    Relevance: ${result.relevance}
                </span>

                <span class="score">
                    Personalization: ${result.personalization}
                </span>

                <span class="score final">
                    Final Score: ${result.finalScore}
                </span>

            </div>
        `;

        resultsContainer.appendChild(card);
    });

    resultsSection.classList.remove("hidden");

    resultsSection.scrollIntoView({
        behavior: "smooth"
    });
}


// ==========================================
// SPELLING SUGGESTION
// ==========================================

function getSuggestion(query) {

    const corrections = {
        internt: "internet",
        interneet: "internet",
        dnss: "dns",
        protcol: "protocol"
    };

    return corrections[query] || null;
}


// ==========================================
// SEARCH
// ==========================================

function performSearch() {

    const query =
        searchInput.value.trim().toLowerCase();

    if (query === "") {
        searchInput.focus();
        return;
    }

    suggestion.textContent = "";

    const correction =
        getSuggestion(query);

    if (correction) {

        suggestion.innerHTML =
            `Did you mean <strong>${correction}</strong>?`;
    }

    if (demoResults[query]) {

        displayResults(
            query,
            demoResults[query]
        );

        return;
    }

    resultsTitle.textContent =
        `Results for "${query}"`;

    resultCount.textContent =
        "0 results";

    resultsContainer.innerHTML = `
        <article class="result-card">

            <div class="result-title">
                No results found
            </div>

            <div class="result-snippet">
                The real C++ search engine will be
                connected in the next integration stage.
            </div>

        </article>
    `;

    resultsSection.classList.remove("hidden");
}


// ==========================================
// EVENTS
// ==========================================

searchButton.addEventListener(
    "click",
    performSearch
);

searchInput.addEventListener(
    "keydown",
    (event) => {

        if (event.key === "Enter") {
            performSearch();
        }

    }
);

clearButton.addEventListener(
    "click",
    () => {

        searchInput.value = "";

        suggestion.textContent = "";

        resultsSection.classList.add(
            "hidden"
        );

        searchInput.focus();
    }
);


document
    .querySelectorAll(".quick-search")
    .forEach((button) => {

        button.addEventListener(
            "click",
            () => {

                searchInput.value =
                    button.dataset.query;

                performSearch();
            }
        );

    });


// ==========================================
// 3D CANVAS
// ==========================================

const canvas =
    document.getElementById("threeCanvas");

const ctx =
    canvas.getContext("2d");


let width =
    window.innerWidth;

let height =
    window.innerHeight;


canvas.width = width;
canvas.height = height;


// ==========================================
// MOUSE
// ==========================================

let mouseX = 0;
let mouseY = 0;

window.addEventListener(
    "mousemove",
    (event) => {

        mouseX =
            event.clientX / width - 0.5;

        mouseY =
            event.clientY / height - 0.5;

    }
);


// ==========================================
// PARTICLES
// ==========================================

const particles = [];

const particleCount = 220;


for (
    let i = 0;
    i < particleCount;
    i++
) {

    const radius =
        180 + Math.random() * 280;

    const theta =
        Math.random() * Math.PI * 2;

    const phi =
        Math.acos(
            2 * Math.random() - 1
        );


    particles.push({

        x:
            radius *
            Math.sin(phi) *
            Math.cos(theta),

        y:
            radius *
            Math.sin(phi) *
            Math.sin(theta),

        z:
            radius *
            Math.cos(phi)

    });

}


// ==========================================
// ROTATION
// ==========================================

let rotationY = 0;
let rotationX = 0;


// ==========================================
// PROJECT 3D POINT
// ==========================================

function projectPoint(point) {

    let x = point.x;
    let y = point.y;
    let z = point.z;


    const cosY =
        Math.cos(rotationY);

    const sinY =
        Math.sin(rotationY);


    const rotatedX =
        x * cosY - z * sinY;

    const rotatedZ =
        x * sinY + z * cosY;


    x = rotatedX;
    z = rotatedZ;


    const cosX =
        Math.cos(rotationX);

    const sinX =
        Math.sin(rotationX);


    const rotatedY =
        y * cosX - z * sinX;

    const finalZ =
        y * sinX + z * cosX;


    y = rotatedY;
    z = finalZ;


    const perspective =
        600 / (600 + z);


    return {

        x:
            width / 2 +
            x * perspective,

        y:
            height / 2 +
            y * perspective,

        scale:
            perspective,

        z:
            z

    };
}


// ==========================================
// DRAW 3D BACKGROUND
// ==========================================

function draw3D() {

    ctx.clearRect(
        0,
        0,
        width,
        height
    );


    // Background glow

    const glow =
        ctx.createRadialGradient(
            width / 2,
            height / 2,
            20,
            width / 2,
            height / 2,
            Math.min(width, height) * 0.55
        );


    glow.addColorStop(
        0,
        "rgba(100,110,255,0.12)"
    );

    glow.addColorStop(
        0.5,
        "rgba(70,80,180,0.035)"
    );

    glow.addColorStop(
        1,
        "rgba(0,0,0,0)"
    );


    ctx.fillStyle = glow;

    ctx.fillRect(
        0,
        0,
        width,
        height
    );


    rotationY += 0.0018;


    rotationX =
        mouseY * 0.08;


    const projected =
        particles.map(
            projectPoint
        );


    // ======================================
    // CONNECTIONS
    // ======================================

    for (
        let i = 0;
        i < projected.length;
        i++
    ) {

        for (
            let j = i + 1;
            j < projected.length;
            j++
        ) {

            const p1 =
                projected[i];

            const p2 =
                projected[j];


            const dx =
                p1.x - p2.x;

            const dy =
                p1.y - p2.y;


            const distance =
                Math.sqrt(
                    dx * dx +
                    dy * dy
                );


            if (
                distance < 95
            ) {

                const opacity =
                    (
                        1 -
                        distance / 95
                    ) * 0.16;


                ctx.strokeStyle =
                    `rgba(140,150,255,${opacity})`;

                ctx.lineWidth = 0.5;


                ctx.beginPath();

                ctx.moveTo(
                    p1.x,
                    p1.y
                );

                ctx.lineTo(
                    p2.x,
                    p2.y
                );

                ctx.stroke();

            }

        }

    }


    // ======================================
    // PARTICLES
    // ======================================

    projected.forEach(
        (point) => {

            if (
                point.x < -10 ||
                point.x > width + 10 ||
                point.y < -10 ||
                point.y > height + 10
            ) {

                return;

            }


            const size =
                Math.max(
                    0.6,
                    2.2 * point.scale
                );


            const opacity =
                Math.max(
                    0.15,
                    Math.min(
                        0.8,
                        point.scale
                    )
                );


            ctx.beginPath();


            ctx.arc(
                point.x,
                point.y,
                size,
                0,
                Math.PI * 2
            );


            ctx.fillStyle =
                `rgba(170,180,255,${opacity})`;


            ctx.fill();

        }
    );


    requestAnimationFrame(
        draw3D
    );

}


draw3D();


// ==========================================
// RESIZE
// ==========================================

window.addEventListener(
    "resize",
    () => {

        width =
            window.innerWidth;

        height =
            window.innerHeight;

        canvas.width =
            width;

        canvas.height =
            height;

    }
);
