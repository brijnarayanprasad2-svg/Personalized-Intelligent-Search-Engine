// =========================================================
// FRONTEND ELEMENTS
// =========================================================

const searchInput =
    document.getElementById("searchInput");

const searchButton =
    document.getElementById("searchButton");

const clearButton =
    document.getElementById("clearButton");

const suggestion =
    document.getElementById("suggestion");

const resultsSection =
    document.getElementById("resultsSection");

const resultsContainer =
    document.getElementById("resultsContainer");

const resultsTitle =
    document.getElementById("resultsTitle");

const resultCount =
    document.getElementById("resultCount");


// =========================================================
// C++ API CONFIGURATION
// =========================================================

const API_BASE_URL =
    "http://127.0.0.1:8080";

// =========================================================
// RESULT CARD INTERACTION STYLE
// =========================================================

(function addResultInteractionStyles()
{
    if (document.getElementById("piseResultInteractionStyles"))
    {
        return;
    }

    const style = document.createElement("style");
    style.id = "piseResultInteractionStyles";
    style.textContent = `
        .clickable-result {
            cursor: pointer;
            transition: transform .22s ease, border-color .22s ease, box-shadow .22s ease;
        }

        .clickable-result:hover {
            transform: translateY(-3px);
            border-color: rgba(150, 160, 255, .42) !important;
            box-shadow: 0 18px 45px rgba(40, 48, 110, .20);
        }

        .clickable-result:focus-visible {
            outline: 2px solid rgba(150, 160, 255, .85);
            outline-offset: 4px;
        }

        .result-open {
            margin-top: 16px;
            padding-top: 13px;
            border-top: 1px solid rgba(255,255,255,.07);
            color: #aeb7ff;
            font-size: 12px;
            font-weight: 700;
            letter-spacing: .02em;
        }

        .result-open span {
            display: inline-block;
            margin-left: 5px;
            transition: transform .2s ease;
        }

        .clickable-result:hover .result-open span {
            transform: translateX(4px);
        }
    `;
    document.head.appendChild(style);
})();



// =========================================================
// SEARCH REQUEST CONTROL
// =========================================================

let latestSearchRequestId = 0;

let activeAbortController = null;


// =========================================================
// BASIC ELEMENT VALIDATION
// =========================================================

function requiredElement(element, name)
{
    if (!element)
    {
        console.error(
            `${name} element was not found in the HTML.`
        );

        return false;
    }

    return true;
}


// =========================================================
// HTML ESCAPING
// =========================================================
// Prevents HTML injection when displaying backend data.

function escapeHtml(value)
{
    const div =
        document.createElement("div");

    div.textContent =
        String(value ?? "");

    return div.innerHTML;
}


// =========================================================
// FRONTEND QUERY NORMALIZATION
// =========================================================

function normalizeFrontendQuery(query)
{
    return String(query ?? "")
        .trim()
        .toLowerCase()
        .replace(/\s+/g, " ");
}


// =========================================================
// OPEN EXTERNAL WEBSITE
// =========================================================

function openExternalWebsite(url)
{
    window.open(
        url,
        "_blank",
        "noopener,noreferrer"
    );
}


// =========================================================
// EXTERNAL WEBSITE NAVIGATION
// =========================================================
// Recognized navigation examples:
//
// youtube
// youtube music
// google
// google c++
// github
// github trie
//
// All other queries continue to the C++ search engine.

function handleExternalNavigation(query)
{
    const normalizedQuery =
        normalizeFrontendQuery(query);


    // =====================================================
    // YOUTUBE
    // =====================================================

    if (normalizedQuery === "youtube")
    {
        openExternalWebsite(
            "https://www.youtube.com/"
        );

        return true;
    }


    // YouTube search
    // Example:
    // youtube music
    // youtube c++ tutorial

    if (normalizedQuery.startsWith("youtube "))
    {
        const searchTerm =
            query
                .trim()
                .substring(7)
                .trim();

        if (searchTerm)
        {
            openExternalWebsite(
                "https://www.youtube.com/results?search_query=" +
                encodeURIComponent(searchTerm)
            );

            return true;
        }
    }


    // =====================================================
    // GOOGLE
    // =====================================================

    if (normalizedQuery === "google")
    {
        openExternalWebsite(
            "https://www.google.com/"
        );

        return true;
    }


    // Google search
    // Example:
    // google c++

    if (normalizedQuery.startsWith("google "))
    {
        const searchTerm =
            query
                .trim()
                .substring(6)
                .trim();

        if (searchTerm)
        {
            openExternalWebsite(
                "https://www.google.com/search?q=" +
                encodeURIComponent(searchTerm)
            );

            return true;
        }
    }


    // =====================================================
    // GITHUB
    // =====================================================

    if (normalizedQuery === "github")
    {
        openExternalWebsite(
            "https://github.com/"
        );

        return true;
    }


    // GitHub search
    // Example:
    // github trie

    if (normalizedQuery.startsWith("github "))
    {
        const searchTerm =
            query
                .trim()
                .substring(6)
                .trim();

        if (searchTerm)
        {
            openExternalWebsite(
                "https://github.com/search?q=" +
                encodeURIComponent(searchTerm)
            );

            return true;
        }
    }


    // =====================================================
    // NORMAL INTERNAL SEARCH
    // =====================================================

    return false;
}

// =========================================================
// GET DETAILS PAGE KEY
// =========================================================

function getDetailsPageKey(result)
{
    const url =
        String(result?.url ?? "")
            .trim()
            .toLowerCase();

    const title =
        String(result?.title ?? "")
            .trim()
            .toLowerCase();


    // -----------------------------------------------------
    // Extract filename from URL
    // -----------------------------------------------------

    let value = url;

    value = value.split("?")[0];
    value = value.split("#")[0];
    value = value.split("/").pop();

    value = value.replace(
        /\.html$/i,
        ""
    );

    value = value
        .replace(/[-\s]+/g, "_")
        .replace(/[^a-z0-9_]/g, "");


    // -----------------------------------------------------
    // Known PISE pages
    // -----------------------------------------------------

    const knownPages =
    {
        "computer_science":
            "computer_science",

        "data_structures":
            "data_structures",

        "algorithms":
            "algorithms",

        "cpp_programming":
            "cpp_programming",

        "object_oriented_programming":
            "object_oriented_programming",

        "data_science":
            "data_science",

        "machine_learning":
            "machine_learning",

        "artificial_intelligence":
            "artificial_intelligence",

        "internet_protocols":
            "internet_protocols",

        "database_management":
            "database_management"
    };


    // -----------------------------------------------------
    // Match URL
    // -----------------------------------------------------

    if (knownPages[value])
    {
        return knownPages[value];
    }


    // -----------------------------------------------------
    // Match title
    // -----------------------------------------------------

    const normalizedTitle =
        title
            .replace(/[^a-z0-9\s]/g, "")
            .replace(/\s+/g, "_");


    if (knownPages[normalizedTitle])
    {
        return knownPages[normalizedTitle];
    }


    // -----------------------------------------------------
    // Partial title matching
    // -----------------------------------------------------

    if (title.includes("data science"))
    {
        return "data_science";
    }


    if (title.includes("data structure"))
    {
        return "data_structures";
    }


    if (title.includes("machine learning"))
    {
        return "machine_learning";
    }


    if (title.includes("artificial intelligence"))
    {
        return "artificial_intelligence";
    }


    if (title.includes("object oriented"))
    {
        return "object_oriented_programming";
    }


    if (title.includes("computer science"))
    {
        return "computer_science";
    }


    if (title.includes("database"))
    {
        return "database_management";
    }


    if (title.includes("algorithm"))
    {
        return "algorithms";
    }


    if (
        title.includes("internet") ||
        title.includes("protocol")
    )
    {
        return "internet_protocols";
    }


    if (
        title.includes("c++") ||
        title.includes("cpp")
    )
    {
        return "cpp_programming";
    }


    return null;
}
// =========================================================
// DISPLAY SEARCH RESULTS
// =========================================================

function displayResults(
    query,
    results
)
{
    if (!requiredElement(
        resultsContainer,
        "resultsContainer"
    ))
    {
        return;
    }


    // -----------------------------------------------------
    // CLEAR OLD RESULTS
    // -----------------------------------------------------

    resultsContainer.innerHTML = "";


    // -----------------------------------------------------
    // UPDATE HEADING
    // -----------------------------------------------------

    if (resultsTitle)
    {
        resultsTitle.textContent =
            `Results for "${query}"`;
    }


    // -----------------------------------------------------
    // UPDATE RESULT COUNT
    // -----------------------------------------------------

    if (resultCount)
    {
        resultCount.textContent =
            `${results.length} result${
                results.length === 1
                    ? ""
                    : "s"
            }`;
    }


    // -----------------------------------------------------
    // NO RESULTS
    // -----------------------------------------------------

    if (results.length === 0)
    {
        const emptyCard =
            document.createElement("article");

        emptyCard.className =
            "result-card";


        emptyCard.innerHTML = `
            <div class="result-title">
                No results found
            </div>

            <div class="result-snippet">
                No matching webpages were found
                in the indexed pages.
            </div>
        `;


        resultsContainer.appendChild(
            emptyCard
        );


        if (resultsSection)
        {
            resultsSection.classList.remove(
                "hidden"
            );
        }


        return;
    }


    // -----------------------------------------------------
    // DISPLAY RESULTS
    // -----------------------------------------------------

    results.forEach(
        (result, index) =>
        {
            const card =
                document.createElement("article");


            card.className =
                "result-card";


            // -------------------------------------------------
            // SAFE VALUE EXTRACTION
            // -------------------------------------------------

            const title =
                result?.title ??
                "Untitled page";


            const url =
                result?.url ??
                "";


            const snippet =
                result?.snippet ??
                "";


            const relevance =
                result?.relevance ??
                0;


            const personalization =
                result?.personalization ??
                0;


            const finalScore =
                result?.finalScore ??
                0;


            // -------------------------------------------------
            // FIND DETAILS PAGE
            // -------------------------------------------------

            const detailsKey =
                getDetailsPageKey(result);


            // -------------------------------------------------
            // RESULT CARD HTML
            // -------------------------------------------------

            card.innerHTML = `
                <div class="result-title">
                    ${index + 1}.
                    ${escapeHtml(title)}
                </div>

                <div class="result-url">
                    ${escapeHtml(url)}
                </div>

                <div class="result-snippet">
                    ${escapeHtml(snippet)}
                </div>

                <div class="result-meta">

                    <span class="score">
                        Relevance:
                        ${escapeHtml(relevance)}
                    </span>

                    <span class="score">
                        Personalization:
                        ${escapeHtml(personalization)}
                    </span>

                    <span class="score final">
                        Final Score:
                        ${escapeHtml(finalScore)}
                    </span>

                </div>

                ${
                    detailsKey
                        ? `
                            <div class="result-action">
                                Read full article
                                <span>→</span>
                            </div>
                          `
                        : ""
                }
            `;


            // -------------------------------------------------
            // MAKE CARD CLICKABLE
            // -------------------------------------------------

            if (detailsKey)
            {
                card.classList.add(
                    "result-card-clickable"
                );


                card.setAttribute(
                    "role",
                    "button"
                );


                card.setAttribute(
                    "tabindex",
                    "0"
                );


                card.setAttribute(
                    "aria-label",
                    `Read full article about ${title}`
                );


                // ---------------------------------------------
                // Mouse click
                // ---------------------------------------------

                card.addEventListener(
                    "click",
                    () =>
                    {
                        window.location.href =
                            `./details.html?page=${
                                encodeURIComponent(
                                    detailsKey
                                )
                            }`;
                    }
                );


                // ---------------------------------------------
                // Keyboard accessibility
                // ---------------------------------------------

                card.addEventListener(
                    "keydown",
                    (event) =>
                    {
                        if (
                            event.key === "Enter" ||
                            event.key === " "
                        )
                        {
                            event.preventDefault();

                            window.location.href =
                                `./details.html?page=${
                                    encodeURIComponent(
                                        detailsKey
                                    )
                                }`;
                        }
                    }
                );
            }


            resultsContainer.appendChild(
                card
            );
        }
    );


    // -----------------------------------------------------
    // SHOW RESULTS SECTION
    // -----------------------------------------------------

    if (resultsSection)
    {
        resultsSection.classList.remove(
            "hidden"
        );


        resultsSection.scrollIntoView(
            {
                behavior: "smooth",
                block: "start"
            }
        );
    }
}


// =========================================================
// SHOW LOADING STATE
// =========================================================

function showLoadingState()
{
    if (resultsSection)
    {
        resultsSection.classList.remove(
            "hidden"
        );
    }


    if (resultsTitle)
    {
        resultsTitle.textContent =
            "Searching...";
    }


    if (resultCount)
    {
        resultCount.textContent =
            "";
    }


    if (resultsContainer)
    {
        resultsContainer.innerHTML = `
            <article class="result-card">

                <div class="result-title">
                    Searching the C++ engine...
                </div>

                <div class="result-snippet">
                    Processing your query, checking the
                    search index and ranking results.
                </div>

            </article>
        `;
    }
}


// =========================================================
// SHOW API ERROR
// =========================================================

function showApiError(
    query,
    error
)
{
    console.error(
        "Search API Error:",
        error
    );


    if (resultsTitle)
    {
        resultsTitle.textContent =
            `Results for "${query}"`;
    }


    if (resultCount)
    {
        resultCount.textContent =
            "0 results";
    }


    if (resultsContainer)
    {
        resultsContainer.innerHTML = `
            <article class="result-card">

                <div class="result-title">
                    Search service unavailable
                </div>

                <div class="result-snippet">
                    Could not connect to the C++
                    search engine API.

                    <br><br>

                    Make sure the API server is running
                    at ${escapeHtml(API_BASE_URL)}.
                </div>

            </article>
        `;
    }


    if (resultsSection)
    {
        resultsSection.classList.remove(
            "hidden"
        );
    }
}


// =========================================================
// SEARCH USING C++ API
// =========================================================

async function performSearch()
{
    // =====================================================
    // VALIDATE SEARCH INPUT
    // =====================================================

    if (!searchInput)
    {
        console.error(
            "Search input element not found."
        );

        return;
    }


    // =====================================================
    // READ QUERY
    // =====================================================

    const query =
        searchInput.value.trim();


    // =====================================================
    // EMPTY QUERY
    // =====================================================

    if (!query)
    {
        searchInput.focus();

        return;
    }


    // =====================================================
    // EXTERNAL WEBSITE NAVIGATION
    // =====================================================

    if (
        handleExternalNavigation(query)
    )
    {
        return;
    }


    // =====================================================
    // CREATE REQUEST ID
    // =====================================================

    const requestId =
        ++latestSearchRequestId;


    // =====================================================
    // CANCEL PREVIOUS REQUEST
    // =====================================================

    if (activeAbortController)
    {
        activeAbortController.abort();
    }


    activeAbortController =
        new AbortController();


    // =====================================================
    // CLEAR OLD SUGGESTION
    // =====================================================

    if (suggestion)
    {
        suggestion.textContent =
            "";
    }


    // =====================================================
    // SHOW LOADING
    // =====================================================

    showLoadingState();


    // =====================================================
    // DISABLE SEARCH BUTTON
    // =====================================================

    if (searchButton)
    {
        searchButton.disabled =
            true;
    }


    try
    {
        // =================================================
        // BUILD API URL
        // =================================================

        const apiURL =
            `${API_BASE_URL}/search?q=${
                encodeURIComponent(query)
            }`;


        // =================================================
        // SEND API REQUEST
        // =================================================

        const response =
            await fetch(
                apiURL,
                {
                    method: "GET",

                    headers:
                    {
                        "Accept":
                            "application/json"
                    },

                    signal:
                        activeAbortController.signal
                }
            );


        // =================================================
        // IGNORE OUTDATED REQUEST
        // =================================================

        if (
            requestId !==
            latestSearchRequestId
        )
        {
            return;
        }


        // =================================================
        // HTTP ERROR
        // =================================================

        if (!response.ok)
        {
            throw new Error(
                `HTTP ${response.status}`
            );
        }


        // =================================================
        // READ JSON
        // =================================================

        const data =
            await response.json();


        // =================================================
        // API ERROR
        // =================================================

        if (data?.error)
        {
            throw new Error(
                data.error
            );
        }


        // =================================================
        // GET RESULTS
        // =================================================

        const results =
            Array.isArray(data?.results)
                ? data.results
                : [];


        // =================================================
        // FINAL PROCESSED QUERY
        // =================================================

        const displayQuery =
            data?.processedQuery ||
            data?.correctedQuery ||
            query;


        // =================================================
        // SPELLING CORRECTION MESSAGE
        // =================================================
        //
        // Original user input remains unchanged.
        //
        // Example:
        //
        // Search box:
        // protcol
        //
        // Result:
        // protocol

        if (
            data?.queryWasCorrected &&
            data?.correctedQuery &&
            data.correctedQuery !== query
        )
        {
            if (suggestion)
            {
                suggestion.textContent =
                    `Showing results for "${data.correctedQuery}"`;
            }
        }
        else
        {
            if (suggestion)
            {
                suggestion.textContent =
                    "";
            }
        }
        // =========================================================
        // RESULT PAGE KEY
        // =========================================================
        // Converts the backend page URL/title into the topic key 
        // =========================================================

        function getDetailsPageKey(result)
        {
           const url =
            String(result?.url ?? "")
            .trim()
            .toLowerCase();

            const title =
            String(result?.title ?? "")
            .trim()
            .toLowerCase();


            // -----------------------------------------------------
            // First try the backend URL
           // -----------------------------------------------------

           let value = url;


            // Remove query string
             value =
               value.split("?")[0];


            // Remove hash
             value =
               value.split("#")[0];


            // Remove path
            value =
               value.split("/").pop();

            // Remove .html
            value =
              value.replace(/\.html$/i, "");

            // Replace spaces/hyphens with underscore
              value =
               value
                .replace(/[-\s]+/g, "_")
                .replace(/[^a-z0-9_]/g, "");


            // -----------------------------------------------------
            // Known project pages
             // -----------------------------------------------------

           const knownPages =
            {
               "computer_science":
               "computer_science",

                "data_structures":
                "data_structures",

                "algorithms":
                "algorithms",

                "cpp_programming":
                "cpp_programming",
 
                "object_oriented_programming":
                "object_oriented_programming",

                "data_science":
                "data_science",

                "machine_learning":
                "machine_learning",

                "artificial_intelligence":
                "artificial_intelligence",

                "internet_protocols":
                "internet_protocols",

                "database_management":
                "database_management"
            };


           // -----------------------------------------------------
           // Direct URL match
           // -----------------------------------------------------

            if (knownPages[value])
            {
               return knownPages[value];
            }


               // -----------------------------------------------------
              // Title-based fallback
               // -----------------------------------------------------

            const normalizedTitle =
                 title
                 .replace(/[^a-z0-9\s]/g, "")
                  .replace(/\s+/g, "_");


                if (knownPages[normalizedTitle])
                {
                   return knownPages[normalizedTitle];
                }


              // -----------------------------------------------------
             // Partial title matching
              // -----------------------------------------------------

            if(
              title.includes("data science")
            )
            {
             return "data_science";
            }

            if(
                title.includes("data structure")
            )
            {
               return "data_structures";
            }

            if(
               title.includes("machine learning")
            )
            {
               return "machine_learning";
            }

            if(
               title.includes("artificial intelligence")
           )
           { 
               return "artificial_intelligence";
            }

            if(
                title.includes("object oriented")
            )
            {
               return "object_oriented_programming";
            }

            if(
                  title.includes("computer science")
            )
            {
              return "computer_science";
            }

            if(
                 title.includes("database")
            )
            {
              return "database_management";
            }


            if( 
               title.includes("algorithm")
            )
            {
              return "algorithms";
            }

            if(
               title.includes("internet") ||
               title.includes("protocol")
            )
            {
              return "internet_protocols";
            }

            if (
              title.includes("c++") ||
              title.includes("cpp")
            )
            {
               return "cpp_programming";
            }


              // -----------------------------------------------------
              // No known topic
               // -----------------------------------------------------

             return null;
        }


        // =================================================
        // DISPLAY RESULTS
        // =================================================

        displayResults(
            displayQuery,
            results
        );
    }
    catch (error)
    {
        // =================================================
        // IGNORE CANCELED REQUESTS
        // =================================================

        if (
            error?.name === "AbortError"
        )
        {
            return;
        }


        // =================================================
        // IGNORE OUTDATED REQUESTS
        // =================================================

        if (
            requestId !==
            latestSearchRequestId
        )
        {
            return;
        }


        // =================================================
        // SHOW ERROR
        // =================================================

        showApiError(
            query,
            error
        );
    }
    finally
    {
        // Only reset the active controller if this
        // request is still the latest one.

        if (
            requestId ===
            latestSearchRequestId
        )
        {
            activeAbortController =
                null;


            if (searchButton)
            {
                searchButton.disabled =
                    false;
            }
        }
    }
}


// =========================================================
// CLEAR SEARCH
// =========================================================

if (clearButton)
{
    clearButton.addEventListener(
        "click",
        () =>
        {
            // Invalidate all previous requests
            latestSearchRequestId++;


            // Cancel active request
            if (activeAbortController)
            {
                activeAbortController.abort();

                activeAbortController =
                    null;
            }


            // Clear input
            if (searchInput)
            {
                searchInput.value =
                    "";
            }


            // Clear suggestion
            if (suggestion)
            {
                suggestion.textContent =
                    "";
            }


            // Clear results
            if (resultsContainer)
            {
                resultsContainer.innerHTML =
                    "";
            }


            // Clear count
            if (resultCount)
            {
                resultCount.textContent =
                    "";
            }


            // Hide results
            if (resultsSection)
            {
                resultsSection.classList.add(
                    "hidden"
                );
            }


            // Reset search button
            if (searchButton)
            {
                searchButton.disabled =
                    false;
            }


            // Focus input
            if (searchInput)
            {
                searchInput.focus();
            }
        }
    );
}


// =========================================================
// SEARCH BUTTON
// =========================================================

if (searchButton)
{
    searchButton.addEventListener(
        "click",
        performSearch
    );
}


// =========================================================
// ENTER KEY SEARCH
// =========================================================

if (searchInput)
{
    searchInput.addEventListener(
        "keydown",
        (event) =>
        {
            if (event.key === "Enter")
            {
                event.preventDefault();

                performSearch();
            }
        }
    );
}


// =========================================================
// QUICK SEARCH BUTTONS
// =========================================================

document
    .querySelectorAll(".quick-search")
    .forEach(
        (button) =>
        {
            button.addEventListener(
                "click",
                () =>
                {
                    const quickQuery =
                        button.dataset.query;


                    if (!quickQuery)
                    {
                        return;
                    }


                    if (searchInput)
                    {
                        searchInput.value =
                            quickQuery;

                        searchInput.focus();
                    }


                    performSearch();
                }
            );
        }
    );


// =========================================================
// 3D BACKGROUND
// =========================================================

const canvas =
    document.getElementById("threeCanvas");


// =========================================================
// 3D CANVAS VALIDATION
// =========================================================

let ctx = null;

if (canvas)
{
    ctx =
        canvas.getContext("2d");


    if (!ctx)
    {
        console.error(
            "Could not create 2D canvas context."
        );
    }
}
else
{
    console.error(
        'Canvas with id="threeCanvas" was not found.'
    );
}


// =========================================================
// CANVAS DIMENSIONS
// =========================================================

let width =
    window.innerWidth;

let height =
    window.innerHeight;

let centerX =
    width / 2;

let centerY =
    height / 2;


// =========================================================
// SETUP CANVAS
// =========================================================

function resizeCanvas()
{
    if (!canvas)
    {
        return;
    }


    width =
        window.innerWidth;

    height =
        window.innerHeight;


    centerX =
        width / 2;

    centerY =
        height / 2;


    canvas.width =
        width;

    canvas.height =
        height;


    // Keep canvas as a non-interactive background layer.
    canvas.style.position =
        "fixed";

    canvas.style.top =
        "0";

    canvas.style.left =
        "0";

    canvas.style.width =
        "100vw";

    canvas.style.height =
        "100vh";

    canvas.style.pointerEvents =
        "none";

    canvas.style.zIndex =
        "0";
}

resizeCanvas();


// =========================================================
// MOUSE
// =========================================================

let mouseX = 0;

let mouseY = 0;


window.addEventListener(
    "mousemove",
    (event) =>
    {
        if (width > 0)
        {
            mouseX =
                (
                    event.clientX /
                    width
                ) - 0.5;
        }


        if (height > 0)
        {
            mouseY =
                (
                    event.clientY /
                    height
                ) - 0.5;
        }
    }
);

// =========================================================
// 3D PARTICLES
// =========================================================

const particles = [];

const particleCount =
    260;


if (canvas && ctx)
{
    for (
        let i = 0;
        i < particleCount;
        i++
    )
    {
        const radius =
            180 +
            Math.random() * 260;


        const theta =
            Math.random() *
            Math.PI *
            2;


        const phi =
            Math.acos(
                2 * Math.random() - 1
            );


        particles.push(
            {
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
            }
        );
    }
}

// =========================================================
// ROTATION
// =========================================================

let rotationY = 0;

let rotationX = 0;


// =========================================================
// 3D PROJECTION
// =========================================================

function project3D(point)
{
    let x =
        point.x;

    let y =
        point.y;

    let z =
        point.z;


    // =====================================================
    // ROTATION AROUND Y
    // =====================================================

    const cosY =
        Math.cos(rotationY);

    const sinY =
        Math.sin(rotationY);


    const rotatedX =
        x * cosY -
        z * sinY;


    const rotatedZ =
        x * sinY +
        z * cosY;


    x =
        rotatedX;

    z =
        rotatedZ;


    // =====================================================
    // ROTATION AROUND X
    // =====================================================

    const cosX =
        Math.cos(rotationX);

    const sinX =
        Math.sin(rotationX);


    const rotatedY =
        y * cosX -
        z * sinX;


    const finalZ =
        y * sinX +
        z * cosX;


    y =
        rotatedY;

    z =
        finalZ;


    // =====================================================
    // MOUSE MOVEMENT
    // =====================================================

    x +=
        mouseX * 80;

    y +=
        mouseY * 50;


    // =====================================================
    // SAFE PERSPECTIVE
    // =====================================================

    const denominator =
        600 + z;

    const perspective =
        denominator !== 0
            ? 600 / denominator
            : 1;


    return {
        x:
            centerX +
            x * perspective,

        y:
            centerY +
            y * perspective,

        scale:
            perspective,

        z:
            z
    };
}


// =========================================================
// DRAW 3D BACKGROUND
// =========================================================

function draw3DBackground()
{
    // =====================================================
    // STOP ONLY IF CANVAS IS UNAVAILABLE
    // =====================================================

    if (!canvas || !ctx)
    {
        return;
    }


    // =====================================================
    // CLEAR CANVAS
    // =====================================================

    ctx.clearRect(
        0,
        0,
        width,
        height
    );


    // =====================================================
    // BACKGROUND GLOW
    // =====================================================

    const glow =
        ctx.createRadialGradient(
            centerX,
            centerY,
            20,
            centerX,
            centerY,
            Math.min(
                width,
                height
            ) * 0.55
        );


    glow.addColorStop(
        0,
        "rgba(100,110,255,0.10)"
    );


    glow.addColorStop(
        0.45,
        "rgba(70,80,200,0.04)"
    );


    glow.addColorStop(
        1,
        "rgba(0,0,0,0)"
    );


    ctx.fillStyle =
        glow;


    ctx.fillRect(
        0,
        0,
        width,
        height
    );


    // =====================================================
    // ROTATION
    // =====================================================

    rotationY +=
        0.0018;


    rotationX =
        mouseY * 0.08;


    // =====================================================
    // PROJECT PARTICLES
    // =====================================================

    const projected =
        particles.map(
            project3D
        );


    // =====================================================
    // CONNECTION LINES
    // =====================================================

    for (
        let i = 0;
        i < projected.length;
        i++
    )
    {
        const p1 =
            projected[i];


        for (
            let j = i + 1;
            j < projected.length;
            j++
        )
        {
            const p2 =
                projected[j];


            const dx =
                p1.x -
                p2.x;

            const dy =
                p1.y -
                p2.y;


            const distanceSquared =
                dx * dx +
                dy * dy;


            // Avoid unnecessary square-root calculations.
            if (
                distanceSquared <
                10000
            )
            {
                const distance =
                    Math.sqrt(
                        distanceSquared
                    );


                const opacity =
                    (
                        1 -
                        distance / 100
                    ) * 0.16;


                ctx.strokeStyle =
                    `rgba(140,150,255,${opacity})`;


                ctx.lineWidth =
                    0.5;


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


    // =====================================================
    // DRAW PARTICLES
    // =====================================================

    projected.forEach(
        (point) =>
        {
            if (
                point.x < -20 ||
                point.x > width + 20 ||
                point.y < -20 ||
                point.y > height + 20
            )
            {
                return;
            }


            const size =
                Math.max(
                    0.5,
                    2.2 *
                    point.scale
                );


            const opacity =
                Math.max(
                    0.12,
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


    // =====================================================
    // CONTINUE ANIMATION
    // =====================================================

    requestAnimationFrame(
        draw3DBackground
    );
}


// =========================================================
// START 3D ANIMATION
// =========================================================

if (canvas && ctx)
{
    draw3DBackground();
}


// =========================================================
// RESIZE
// =========================================================

window.addEventListener(
    "resize",
    resizeCanvas
);