"use strict";

const fs = require("fs");
const { readReport, driverAckPath, wait, TERMINAL_STATUSES } = require("./config");

module.exports = {
  pollReport() {
    if (Date.now() >= this.deadline) {
      this.fail("timed out waiting for the Endstone report");
      this.close();
      process.exitCode = 2;
      return;
    }

    const report = readReport(this.options.resultFile);
    if (!report) return;
    if (this.options.runId && report.run_id !== this.options.runId) return;
    if (this.options.worldName && report.world_name !== this.options.worldName) return;
    if (TERMINAL_STATUSES.has(report.status)) {
      if (this.actionInFlight) return;
      const status = report.status;
      this.close();
      if (this.failed) process.exitCode = 2;
      else process.exitCode = status === "passed" ? 0 : 1;
      return;
    }

    const scenario = report.active_scenario;
    if (!scenario || this.actionInFlight || scenario === this.lastScenario) return;
    this.lastScenario = scenario;
    const action = report.active_driver_action;
    if (!action || action === "connect" || !this.spawned) return;
    if (report.active_driver_ready !== true) {
      // The Python fixture may need one server tick to place blocks, mutate inventory, or summon the target actor.
      // Do not consume this scenario until its preparation has completed; otherwise the action would use
      // stale coordinates/items from the previous scenario.
      this.lastScenario = null;
      return;
    }
    this.actionInFlightScenario = scenario;
    console.log("[bedrock] driver action start scenario=" + scenario + " action=" + action);
    const writeActionAck = async (status, error = null) => {
      const payload = {
        schema_version: 1,
        run_id: this.options.runId,
        scenario,
        action,
        status,
        error,
      };
      const ackPath = driverAckPath(this.options.resultFile);
      const temporaryPath = `${ackPath}.${process.pid}.tmp`;
      fs.writeFileSync(temporaryPath, JSON.stringify(payload) + "\n", "utf8");
      for (let attempt = 0; attempt < 50; attempt += 1) {
        try {
          fs.rmSync(ackPath, { force: true });
          fs.renameSync(temporaryPath, ackPath);
          return;
        } catch (error) {
          const retryable = ["EACCES", "EBUSY", "EPERM", "ENOTEMPTY"]
            .includes(error && error.code);
          if (!retryable || attempt === 49) throw error;
          await wait(10);
        }
      }
    };
    const actionPromise = Promise.resolve().then(async () => {
      try {
        await writeActionAck("running");
        await this.runAction(action, report.active_driver_data);
        await writeActionAck("completed");
      } catch (error) {
        const detail = error && error.stack ? error.stack : String(error);
        try {
          await writeActionAck("failed", detail);
        } catch (ackError) {
          this.fail(`${action}: acknowledgement write failed: ${ackError}`);
        }
        this.fail(`${action}: ${detail}`);
      }
    }).finally(() => {
      if (this.actionInFlight === actionPromise) {
        this.actionInFlight = null;
        this.actionInFlightScenario = null;
      }
    });
    this.actionInFlight = actionPromise;
  }
,
};
