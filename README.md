# Node.js REST API on Red Hat OpenShift

A small Express REST API built to explore deploying, operating, and troubleshooting containerised applications on **Red Hat OpenShift**.

This hands-on project covers Git-based deployments, Source-to-Image (S2I) builds, Kubernetes Deployments and Pods, Services and Routes, readiness and liveness probes, and recovery from a deliberately introduced application-health failure.

## Tech stack

- **Node.js** and **Express** — HTTP API
- **Git and GitHub** — source control
- **Red Hat OpenShift on AWS (ROSA)** — application platform
- **OpenShift Source-to-Image (S2I)** — container image builds using a Node.js builder
- **Kubernetes** — deployment, Pod lifecycle, and health probes

## API endpoints

| Method | Path | Description |
| --- | --- | --- |
| `GET` | `/` | Returns a JSON welcome message and application status |
| `GET` | `/health` | Returns an HTTP 200 health response when the demo is healthy |

Example homepage response (message text may differ in the deployed version):

```json
{
  "message": "Welcome Tony and Kate to my first API deployed on OpenShift",
  "developer": "Lucas",
  "status": "success"
}
```

Example health response:

```json
{
  "status": "healthy",
  "message": "Application is running correctly"
}
```

## Run locally

**Prerequisites:** Node.js and npm.

```bash
npm install
npm start
```

The server listens on the port specified by the `PORT` environment variable, or **8080** by default. It binds to `0.0.0.0` so that it can accept connections inside a container.

Test the API:

```bash
curl http://localhost:8080/
curl http://localhost:8080/health
```

On Windows PowerShell, you can also use:

```powershell
Invoke-RestMethod http://localhost:8080/health
```

## Deployment architecture

```text
GitHub repository
       |
       v
OpenShift BuildConfig (Node.js S2I)
       |
       v
Container image in OpenShift internal registry
       |
       v
Kubernetes Deployment -> Pod running Node.js
                              ^
                              |
Public HTTPS request -> Route -> Service (port 8080)
```

- **BuildConfig:** Defines how OpenShift builds an image from the Git repository.
- **Image registry:** Stores the resulting container image.
- **Deployment:** Manages the desired running Pod replicas and rolling updates.
- **Pod:** Runs the Node.js application container.
- **Service:** Provides a stable internal network endpoint for the Pods.
- **Route:** Exposes the API at an external URL.

## Deploy from GitHub to OpenShift

1. Open the OpenShift console and select the target project.
2. Choose **Quick create → Import from Git**.
3. Enter this repository's Git URL.
4. Select the detected **Node.js builder image** and use the **BuildConfig** build option.
5. Choose **Deployment** as the resource type.
6. Set the target port to **8080** and enable **Create a route**.
7. Create the application and monitor the build logs.
8. Once the Pod is **Running and Ready**, open the Route URL and test `/health`.

A Git push does **not** necessarily trigger a new build automatically; that requires the appropriate build trigger or webhook. In this project, subsequent builds were also started manually through OpenShift.

## Health checks

The deployment uses HTTP probes against `GET /health` on port **8080**.

| Setting | Readiness probe | Liveness probe |
| --- | --- | --- |
| Path | `/health` | `/health` |
| Port | `8080` | `8080` |
| Initial delay | 5 seconds | 15 seconds |
| Period | 10 seconds | 10 seconds |
| Timeout | 2 seconds | 2 seconds |
| Failure threshold | 3 | 3 |

**Readiness** controls whether a Pod is eligible to receive traffic through the Service. **Liveness** detects containers that need restarting. A Pod can have a running process while still being unready.

These settings were used for a learning exercise, not tuned for production workloads.

## Troubleshooting experiment: HTTP 500

To understand probe failures and Kubernetes recovery behaviour, I deliberately changed `/health` to return **HTTP 500**:

```javascript
app.get('/health', (req, res) => {
  console.log('ERROR: Simulated health check failure!');
  res.status(500).json({
    status: 'unhealthy',
    error: 'Simulated application failure'
  });
});
```

After committing the change to GitHub and starting a new OpenShift build, I inspected the new Pod's events. OpenShift reported:

```text
Readiness probe failed: HTTP probe failed with statuscode: 500
Liveness probe failed: HTTP probe failed with statuscode: 500
Container ... failed liveness probe, will be restarted
```

### What I observed

- The container image built successfully; the issue was **not** a build failure.
- The new Pod was running a process but failed readiness and liveness checks.
- Repeated liveness failures caused the container to restart.
- During the rolling update, the previous healthy Pod remained available while the new version was failing.

### Recovery

1. Restored `/health` to return HTTP 200.
2. Tested the endpoint locally.
3. Committed and pushed the fix to GitHub.
4. Started a new OpenShift build and rollout.
5. Verified the replacement Pod was running and the API responded normally.

**Key takeaway:** A successful build and a `Running` Pod status do not by themselves prove that an application is healthy or ready to serve traffic. Logs, events, probe results, and endpoint tests provide different pieces of evidence.

## Useful OpenShift CLI commands

These commands are useful for investigating the deployment once the `oc` CLI is installed and authenticated:

```bash
oc project lucas-gabriel-dev
oc get pods
oc get deployments
oc get routes
oc describe pod <pod-name>
oc logs <pod-name>
oc rollout status deployment/openshift-node-demo-git
```

## What I learned

- Deploying a Node.js application from GitHub using OpenShift S2I
- How container images, Deployments, Pods, Services, and Routes work together
- The difference between readiness and liveness probes
- How rolling updates behave when a new version is unhealthy
- How to use build logs and Pod events to narrow down a failure
- How to fix, rebuild, and verify an application after an incident

## Scope

This is an educational demo rather than a production-ready service. It does not include authentication, a database, automated tests, or production-grade observability. The HTTP 500 failure was intentional and has been fixed in the recovered deployment.
