package dev.zernyth.ui;

import java.util.Map;
import java.util.Objects;

import dev.zernyth.api.player.Player;

/**
 * A validated UI action from a player: a pressed button or a submitted modal form.
 *
 * @param player the player who acted; the session belonged to this player
 * @param screen screen identifier
 * @param component component identifier, or empty for a modal-form submission
 * @param action declared action name, for example {@code shop.open}
 * @param values submitted values of a modal form by component identifier (Boolean, Double, String or Integer), empty otherwise
 */
public record UiActionEvent(Player player, String screen, String component, String action, Map<String, Object> values) {
    /**
     * Validates the parts.
     *
     * @param player player
     * @param screen screen
     * @param component component
     * @param action action
     * @param values values
     */
    public UiActionEvent {
        Objects.requireNonNull(player);
        Objects.requireNonNull(screen);
        Objects.requireNonNull(component);
        Objects.requireNonNull(action);
        values = Map.copyOf(values);
    }
}
