# Multi-stage build container layout for Charon framework listener
FROM python:3.11-slim AS runtime-env

# Establish isolated internal workspace boundaries
WORKDIR /app

# Expose the internal network socket tracking parameter matching config.json
EXPOSE 4444

# Copy core engine code assets inside the container path mapping
COPY server.py config.json /app/

# Enforce direct buffer un-buffering to monitor shell streams natively
ENV PYTHONUNBUFFERED=1

# Initialize execution boundary logic
ENTRYPOINT ["python3", "server.py"]
